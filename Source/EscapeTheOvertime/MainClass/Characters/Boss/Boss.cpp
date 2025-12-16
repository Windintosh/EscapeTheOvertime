#include "MainClass/Characters/Boss/Boss.h"
#include "MainClass/Characters/Boss/BossAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
ABoss::ABoss()
{
	// Tick은 켜두되, 이동 로직은 넣지 않습니다.
	PrimaryActorTick.bCanEverTick = true;

	// AI 컨트롤러가 자동으로 빙의되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// (선택사항) 보스 회전 설정 - 컨트롤러가 회전을 제어하도록
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	// AI Controller 연결 확인 (디버깅용)
	ABossAIController* BossAI = Cast<ABossAIController>(GetController());
	if (!BossAI)
	{
		UE_LOG(LogTemp, Error, TEXT("Boss: AI Controller NOT Found!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss: AI Controller Connected. Logic is handled by BT."));
	}
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ★ 중요: 기존의 수동 이동 로직(MoveToNextPatrolPoint 등)을 모두 제거했습니다.
	// 이제 이동은 Behavior Tree의 'Move To' 노드가 담당합니다.
	// 여기에 이동 코드를 남겨두면 AI가 명령 충돌로 버벅거립니다.
}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// [NEW] Behavior Tree에서 호출할 공격 함수
void ABoss::Attack()
{
	if (AttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 몽타주 재생
			AnimInstance->Montage_Play(AttackMontage);

			// 몽타주 종료 시점 바인딩 (필요 시 AI에게 알리기 위함)
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ABoss::OnAttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);

			UE_LOG(LogTemp, Warning, TEXT("Boss: Attacking Player!"));
		}
	}
}

void ABoss::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 공격이 끝났을 때의 후처리 (예: 델리게이트 방송)
	// OnAttackFinished.Broadcast(); // 헤더에 델리게이트 선언 시 사용 가능
	UE_LOG(LogTemp, Warning, TEXT("Boss: Attack Finished."));
}

// ★ 기존의 SetPatrolPoints, IsCloseToTarget, MoveToNextPatrolPoint 함수는
// AIController와 Behavior Tree가 대신 수행하므로 삭제했습니다.