#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "TallLockerDoor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API ATallLockerDoor : public ADoorBase
{
	GENERATED_BODY()
	
public:
	ATallLockerDoor();

	virtual void BeginPlay() override;

};
