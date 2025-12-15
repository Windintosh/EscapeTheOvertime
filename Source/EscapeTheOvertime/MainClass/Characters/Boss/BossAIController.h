#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/PawnSensingComponent.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API ABossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABossAIController();

	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UBlackboardComponent* GetBlackboard() const;

	bool bIsPatrolPointsReady = false;

	//AIPerception Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UAIPerceptionComponent* BossAIPerceptionComponent;

	//set sight config
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float ChaseRadius = 500.f;


protected:
	virtual void BeginPlay() override;

	static const FName PatrolLocationReady;

	virtual void OnPossess(APawn* InPawn) override;

	//static const FName PatrolLocationKey;

	//static const FName PatrolIndexKey;

	void InitializePatrolPoints();

	UFUNCTION()
	void StartChasingPlayer(AActor* PlayerActor);

	UFUNCTION()
	void StopChasingPlayer();

	UFUNCTION()
	void StartSearchingLastLocation();

	//Handler
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

private:

	FVector CurrentTargetLocation;

	static const FName PatrolLocationKey;
	static const FName PatrolIndexKey;
	//class 251202 ~ 251203
	static const FName TargetPlayerKey;
	static const FName CanSeePlayerKey;
	static const FName LastSeenLocationKey;
	static const FName SearchStateKey;
	static const FName LastHeardLocationKey; //1203
	static const FName CanHearPlayerKey;

	//
	UBlackboardComponent* BB = nullptr;
};
