#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "SpeedUpItem.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API ASpeedUpItem : public AItemBase
{
	GENERATED_BODY()
	
protected:

	ASpeedUpItem();

	virtual void ActivateItem(AActor* Activator) override;
};
