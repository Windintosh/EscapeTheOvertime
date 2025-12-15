#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "Keycard.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API AKeycard : public AItemBase
{
	GENERATED_BODY()
	
protected:

	AKeycard();

	virtual void ActivateItem(AActor* Activator) override;



};
