#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "RestroomBoothDoor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API ARestroomBoothDoor : public ADoorBase
{
	GENERATED_BODY()

public:
	ARestroomBoothDoor();

protected:

	virtual void BeginPlay() override;
	
};
