#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_SearchPriority.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API UEnvQueryTest_SearchPriority : public UEnvQueryTest
{
	GENERATED_BODY()
	
public:
	UEnvQueryTest_SearchPriority();
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
