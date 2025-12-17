#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Items/ItemBase.h"
#include "ThrownItem.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API AThrownItem : public AItemBase
{
	GENERATED_BODY()
	
protected:

	AThrownItem();

	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void ActivateItem(AActor* Activator) override;

};
