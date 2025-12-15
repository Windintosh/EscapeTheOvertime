#include "MainClass/Characters/Boss/Boss.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<ABossAIController>(GetController());

	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller Not Found!"));
		return;
	}
	else
	{
		BlackboardComp = AIController->GetBlackboardComponent();
		UE_LOG(LogTemp, Warning, TEXT("Blackboard Initialized: %s"), *BlackboardComp->GetName());
	}


}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//When PatrolPoints are ready, checks through boolean variable
	if (AIController->bIsPatrolPointsReady)
	{
		//Get Target Location from blackboard
		CurrentTargetLocation = BlackboardComp->GetValueAsVector("PatrolLocation");
		PatrolPoints = AIController->PatrolPoints;
		AIController->bIsPatrolPointsReady = false;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Current Location: %s"), *GetActorLocation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Current Target Location: %s"), *CurrentTargetLocation.ToString());

	//Check if the character is close to the target
	if (PatrolPoints.Num() > 0 && IsCloseToTarget())
	{
		//Move to next patrol point
		UE_LOG(LogTemp, Warning, TEXT("MoveToNextPatrolPoint Called at Tick()"));
		MoveToNextPatrolPoint();
	}
}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::SetPatrolPoints(const TArray<FVector>& Points)
{
	PatrolPoints = Points;

	//setup first patrol point
	if (PatrolPoints.Num() > 0)
	{
		if (BlackboardComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetPatrolPoints(): PatrolLocation Updated"));
			BlackboardComp->SetValueAsVector("PatrolLocation", CurrentTargetLocation);
		}
	}
}

bool ABoss::IsCloseToTarget(float AcceptanceDistance)
{
	//calculate distance between current location and target location
	float Distance = FVector::Dist(GetActorLocation(), CurrentTargetLocation);
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f, AcceptanceDistance: %f"), Distance, AcceptanceDistance);

	//if (Distance <= AcceptanceDistance)
	//	return true;
	//else
	//	return false;
	return Distance <= AcceptanceDistance;
}

void ABoss::MoveToNextPatrolPoint()
{
	//is Patrolpoint exist?
	if (PatrolPoints.Num() == 0) return;

	//calculate next index(index gets bigger but limits to specific number)
	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
	UE_LOG(LogTemp, Log, TEXT("PatrolPoints Count: %d, Current Index: %d"), PatrolPoints.Num(), CurrentPatrolIndex);

	CurrentTargetLocation = PatrolPoints[CurrentPatrolIndex]; //
	UE_LOG(LogTemp, Log, TEXT("Next Target: %s"), *CurrentTargetLocation.ToString()); //

	//update new location
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector("PatrolLocation", CurrentTargetLocation);
		UE_LOG(LogTemp, Log, TEXT("PatrolLocation Updated"));
		BlackboardComp->SetValueAsInt("PatrolIndex", CurrentPatrolIndex); //
		UE_LOG(LogTemp, Log, TEXT("PatrolIndex Updated: %d"), CurrentPatrolIndex); //
	}
}

