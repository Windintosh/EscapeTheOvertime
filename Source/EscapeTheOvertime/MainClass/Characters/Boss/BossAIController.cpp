#include "MainClass/Characters/Boss/BossAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
//#include "ESearchState.h"
#include "GameFramework/Character.h"

const FName ABossAIController::PatrolLocationKey = TEXT("PatrolLocation");	
const FName ABossAIController::PatrolIndexKey = TEXT("PatrolIndex");
const FName ABossAIController::TargetPlayerKey = TEXT("TargetPlayer");
const FName ABossAIController::LastSeenLocationKey = TEXT("LastSeenLocation");
const FName ABossAIController::CanSeePlayerKey = TEXT("CanSeePlayer");
//const FName ABossAIController::SearchStateKey = TEXT("SearchState");
const FName ABossAIController::LastHeardLocationKey = TEXT("LastHeardLocation");
const FName ABossAIController::CanHearPlayerKey = TEXT("CanHearPlayer");

ABossAIController::ABossAIController() 
{
	PrimaryActorTick.bCanEverTick = true;

	BossAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 600.0f;
	SightConfig->LoseSightRadius = 700.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	BossAIPerceptionComponent->ConfigureSense(*SightConfig);
	BossAIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->HearingThreshold = 1000.0f;
	PawnSensingComponent->LOSHearingThreshold = 1500.0f;
	PawnSensingComponent->SensingInterval = 1.f;
	PawnSensingComponent->bOnlySensePlayers = false;
	PawnSensingComponent->SightRadius = 600.0f;
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();

	BossAIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABossAIController::OnTargetPerceptionUpdated);

	//GetBlackboardComponent()->SetValueAsEnum(SearchStateKey, (uint8)ESearchState::Idle);

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnHearNoise.RemoveAll(this);
		PawnSensingComponent->OnHearNoise.AddDynamic(this, &ABossAIController::OnHearNoise);
		UE_LOG(LogTemp, Warning, TEXT("PawnSensingComponent OnHearNoise Delegate bound"));
	}
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		UBlackboardComponent* BlackboardComp = Blackboard;
		if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp))
		{
			RunBehaviorTree(BehaviorTree);
		}
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossAIController::InitializePatrolPoints, 0.5f, false);

}

void ABossAIController::InitializePatrolPoints()
{
	PatrolPoints.Empty();

	TArray<AActor*> FoundTargetPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundTargetPoints);

	for (AActor* TargetPoint : FoundTargetPoints)
	{
		PatrolPoints.Add(TargetPoint->GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("PatrolPoints Count: %d"), PatrolPoints.Num());

	if (PatrolPoints.Num() > 0)
	{
		Blackboard->SetValueAsVector(PatrolLocationKey, PatrolPoints[0]);
		Blackboard->SetValueAsInt(PatrolIndexKey, 0);
	}

	bIsPatrolPointsReady = true;
}

void ABossAIController::StartChasingPlayer(AActor* PlayerActor)
{
	BB = GetBlackboardComponent();
	if (BB)
	{
		//BB->SetValueAsEnum(SearchStateKey, (uint8)ESearchState::Chasing);
		BB->SetValueAsObject(TargetPlayerKey, PlayerActor);
		BB->SetValueAsBool(CanSeePlayerKey, true);

	}
}

void ABossAIController::StopChasingPlayer()
{
	if (BB->GetValueAsBool(CanSeePlayerKey)) return;

	BB = GetBlackboardComponent();
	if (BB)
	{
		//BB->SetValueAsEnum(SearchStateKey, (uint8)ESearchState::Idle);
		BB->SetValueAsObject(TargetPlayerKey, nullptr);
		BB->SetValueAsBool(CanSeePlayerKey, false);

	}
}

void ABossAIController::StartSearchingLastLocation()
{
	BB = GetBlackboardComponent();
	if (BB)
	{
		//BB->SetValueAsEnum(SearchStateKey, (uint8)ESearchState::Searching);
		//BB->SetValueAsObject(TargetPlayerKey, nullptr);
		//BB->SetValueAsBool(CanSeePlayerKey, false);

	}
}

void ABossAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter) return;

	if (Actor == static_cast<AActor*>(PlayerCharacter))//Cast<ACharacter>(Actor) == PlayerCharacter
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			bool bCanSeePlayer = Stimulus.WasSuccessfullySensed();

			BB = GetBlackboardComponent();

			if (BB)
			{
				BB->SetValueAsObject(TargetPlayerKey, bCanSeePlayer ? Actor : nullptr);
				BB->SetValueAsBool(CanSeePlayerKey, bCanSeePlayer);

				if (bCanSeePlayer)
				{
					APawn* ControlledPawn = GetPawn();
					float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), Actor->GetActorLocation());
					UE_LOG(LogTemp, Warning, TEXT("Player found, Distance: %f"), Distance);
					if (Distance <= ChaseRadius)
					{
						BB->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);
						StartChasingPlayer(Actor);

						UE_LOG(LogTemp, Warning, TEXT("AI Can See Player - Start Chasing"));
					}
				}
				else
				{
					//ESearchState CurrentState = (ESearchState)BB->GetValueAsEnum(SearchStateKey);
					//if (CurrentState == ESearchState::Chasing)
					//{
					//	if (BB)
					//	{
					//		BB->SetValueAsEnum(SearchStateKey, (uint8)ESearchState::Investigating);
					//		BB->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);
					//	}
					//	UE_LOG(LogTemp, Display, TEXT("AI Lost Player - Searching Last Known Location"));

					//	FTimerHandle StopTimer;
					//	GetWorldTimerManager().SetTimer(StopTimer, this, &APatrolAIController::StopChasingPlayer, 10.0f, false);
					//}

					BB->SetValueAsObject(TargetPlayerKey, nullptr);
					//BB->SetValueAsVector(LastSeenLocationKey, Stimulus.StimulusLocation);

				}
			}
		}
	}
}

void ABossAIController::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	BB = GetBlackboardComponent();


	if (!PlayerCharacter || !BB) return;

	if (PawnInstigator == PlayerCharacter) // if source of the noise is Player character
	{
		//update blackboard data
		UE_LOG(LogTemp, Warning, TEXT("AI heard player noise via PawnSensing"));

		BB->SetValueAsBool(CanHearPlayerKey, true);
		BB->SetValueAsVector(LastHeardLocationKey, Location);

		// if currentstate is idle -> set to investigating
		//ESearchState CurrentState = (ESearchState)BB->GetValueAsEnum(SearchStateKey);
		//if (CurrentState == ESearchState::Idle)
		//{
		//	BB->SetValueAsEnum(SearchStateKey, (uint8)ESearchState::Investigating);
		//	UE_LOG(LogTemp, Warning, TEXT("AI State Changed to Investigating due to noise"));
		//}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() // made a function, literally -> lambda capture
			{
				if (BB)
				{
					BB->SetValueAsBool(CanHearPlayerKey, false);

					//ESearchState CurrentState = (ESearchState)BB->GetValueAsEnum(SearchStateKey);
					//if (CurrentState == ESearchState::Investigating)
					//{
					//	BB->SetValueAsEnum(SearchStateKey, (uint8)ESearchState::Idle);
					//	UE_LOG(LogTemp, Warning, TEXT("AI State Changed to Idle after investigating"));
					//}
				}
			}, 5.0f, false);


	}



}
