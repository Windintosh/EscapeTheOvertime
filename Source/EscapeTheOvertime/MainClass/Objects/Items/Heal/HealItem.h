#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "HealItem.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API AHealItem : public AItemBase
{
	GENERATED_BODY()
	
protected:
	AHealItem();

	virtual void ActivateItem(AActor* Activator) override;
};
