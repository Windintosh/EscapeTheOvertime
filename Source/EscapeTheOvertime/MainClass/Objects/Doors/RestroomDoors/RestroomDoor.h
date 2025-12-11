#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "RestroomDoor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API ARestroomDoor : public ADoorBase
{
	GENERATED_BODY()

public:

	ARestroomDoor();

protected:

	virtual void BeginPlay() override;

	
};
