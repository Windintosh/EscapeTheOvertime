#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearAttackState.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API UBTTask_ClearAttackState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ClearAttackState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
