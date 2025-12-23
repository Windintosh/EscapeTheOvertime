#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "MaxHPUpItem.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API AMaxHPUpItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	AMaxHPUpItem();

protected:
	virtual void ActivateItem(AActor* Activator) override;

	float HealAmount = 20.f;
};
