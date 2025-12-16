#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossAIController.h"
//#include "AIController.h"
#include "Boss.generated.h"

UCLASS()
class ESCAPETHEOVERTIME_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Get Patrol Points
	void SetPatrolPoints(const TArray<FVector>& Points);

private:

	ABossAIController* AIController;

	//Blackboard Component
	UBlackboardComponent* BlackboardComp;

	// Current Target point
	FVector CurrentTargetLocation;

	// Array of Patrol Points
	TArray<FVector> PatrolPoints;

	//Current Index of array
	int32 CurrentPatrolIndex = 0;

	//Checks if character is close to targets
	bool IsCloseToTarget(float AcceptanceDistance = 50.0f);

	//Character moves to next patrol point
	void MoveToNextPatrolPoint();

};
