#include "MainClass/Objects/Doors/RestroomDoors/RestroomDoor.h"
#include "RestroomDoor.h"

ARestroomDoor::ARestroomDoor()
{

}

void ARestroomDoor::BeginPlay()
{
	//
	Super::BeginPlay();

	TargetYaw = InitialYaw - 90.f;
}
