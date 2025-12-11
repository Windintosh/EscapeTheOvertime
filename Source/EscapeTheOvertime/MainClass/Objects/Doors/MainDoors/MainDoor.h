#pragma once

#include "CoreMinimal.h"
#include "MainClass/Objects/Doors/DoorBase.h"
#include "MainDoor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API AMainDoor : public ADoorBase
{
	GENERATED_BODY()
public:

	AMainDoor();

protected:

	virtual void BeginPlay() override;
	

};
