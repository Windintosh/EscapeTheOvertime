#include "MainClass/Characters/Boss/BossAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Character.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h" // 청각 센스 헤더

// 블랙보드 키 정의
const FName ABossAIController::PatrolLocationKey = TEXT("PatrolLocation");
const FName ABossAIController::PatrolIndexKey = TEXT("PatrolIndex");
const FName ABossAIController::TargetPlayerKey = TEXT("TargetPlayer");
const FName ABossAIController::LastSeenLocationKey = TEXT("LastSeenLocation");
const FName ABossAIController::CanSeePlayerKey = TEXT("CanSeePlayer");
const FName ABossAIController::LastHeardLocationKey = TEXT("LastHeardLocation");
const FName ABossAIController::CanHearPlayerKey = TEXT("CanHearPlayer");

ABossAIController::ABossAIController()
{
	PrimaryActorTick.bCanEverTick = true; // 디버깅용 외엔 false

	// 1. Perception 컴포넌트 생성
	BossAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	// 2. 시각 설정 (Sight)
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f; // 감지 반경
	SightConfig->LoseSightRadius = 2000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge(5.0f); // 기억 유지 시간

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// 3. 청각 설정 (Hearing) 
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 3000.0f; // 소리 감지 범위
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// 4. 설정 등록
	BossAIPerceptionComponent->ConfigureSense(*SightConfig);
	BossAIPerceptionComponent->ConfigureSense(*HearingConfig);
	BossAIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();

	// 감각 업데이트 델리게이트 바인딩 (시각+청각 통합 처리)
	if (BossAIPerceptionComponent)
	{
		BossAIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABossAIController::OnTargetPerceptionUpdated);
	}
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 비헤이비어 트리 실행
	if (BehaviorTree)
	{
		UBlackboardComponent* BlackboardComp;
		if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp))
		{
			BB = BlackboardComp; // 멤버 변수 BB 캐싱
			RunBehaviorTree(BehaviorTree);
		}
	}

	// 0.5초 뒤 패트롤 지점 초기화 (월드 로딩 대기)
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossAIController::InitializePatrolPoints, 0.5f, false);
}

void ABossAIController::InitializePatrolPoints()
{
	PatrolPoints.Empty();

	// 월드에 배치된 TargetPoint 액터들을 모두 찾음
	TArray<AActor*> FoundTargetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundTargetPoints);

	for (AActor* TargetPoint : FoundTargetPoints)
	{
		PatrolPoints.Add(TargetPoint->GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("AI: PatrolPoints Found: %d"), PatrolPoints.Num());

	// 첫 번째 패트롤 지점 설정
	if (PatrolPoints.Num() > 0 && BB)
	{
		BB->SetValueAsVector(PatrolLocationKey, PatrolPoints[0]);
		BB->SetValueAsInt(PatrolIndexKey, 0);
	}
}

//  통합 감각 처리 함수 (시각 + 청각)
void ABossAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// 플레이어인지 확인
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter || Actor != PlayerCharacter) return;

	if (!BB) return;

	// 감각 종류 식별
	TSubclassOf<UAISense> SensedClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

	// 1. [시각] 눈으로 봤을 때
	if (SensedClass == UAISense_Sight::StaticClass())
	{
		bool bCanSee = Stimulus.WasSuccessfullySensed();

		BB->SetValueAsBool(CanSeePlayerKey, bCanSee);

		if (bCanSee)
		{
			// 플레이어 발견! -> 추격 대상 설정
			BB->SetValueAsObject(TargetPlayerKey, Actor);
			BB->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);

			// 청각 관련 키 초기화 (눈으로 봤으니 소리 추적보다 우선)
			BB->SetValueAsBool(CanHearPlayerKey, false);

			UE_LOG(LogTemp, Warning, TEXT("AI: Player SIGHTED! Chasing..."));
		}
		else
		{
			// 놓침 -> 추격 대상 해제 (하지만 LastSeenLocation에는 마지막 위치가 남아있음)
			BB->SetValueAsObject(TargetPlayerKey, nullptr);
			UE_LOG(LogTemp, Warning, TEXT("AI: Player LOST."));
		}
	}
	// 2. [청각] 소리를 들었을 때
	else if (SensedClass == UAISense_Hearing::StaticClass())
	{
		// 이미 눈으로 보고 쫓고 있다면 소리는 무시
		if (BB->GetValueAsBool(CanSeePlayerKey)) return;

		if (Stimulus.WasSuccessfullySensed())
		{
			BB->SetValueAsBool(CanHearPlayerKey, true);
			BB->SetValueAsVector(LastHeardLocationKey, Stimulus.StimulusLocation);

			UE_LOG(LogTemp, Warning, TEXT("AI: NOISE Heard at %s"), *Stimulus.StimulusLocation.ToString());

			// 5초 뒤에 청각 상태 해제 (소리난 곳 가서 없으면 다시 순찰 돌게 하기 위해)
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
				{
					if (BB) BB->SetValueAsBool(CanHearPlayerKey, false);
				}, 5.0f, false);
		}
	}
}