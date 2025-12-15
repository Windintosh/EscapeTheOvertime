#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Props/PropBase.h"
#include "KeycardReader.generated.h"

UCLASS()
class ESCAPETHEOVERTIME_API AKeycardReader : public APropBase
{
	GENERATED_BODY()
	
protected:

	AKeycardReader();

	virtual void ActivateProp(AActor* Activator) override;
};
