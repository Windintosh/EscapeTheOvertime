#include "MainClass/Objects/Doors/RestroomDoors/RestroomDoor.h"
#include "RestroomDoor.h"

ARestroomDoor::ARestroomDoor()
{

}

void ARestroomDoor::BeginPlay()
{
	//
	Super::BeginPlay();
	if ((-270.f <= GetActorRotation().Yaw && GetActorRotation().Yaw <= -90.f) || (90.f <= GetActorRotation().Yaw && GetActorRotation().Yaw <= 270.f))
	{
		TargetYaw = InitialYaw + 90.f;
	}

	else {
		TargetYaw = InitialYaw - 90.f;
	}
}
