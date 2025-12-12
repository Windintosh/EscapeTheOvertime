#include "MainClass/Objects/Doors/RestroomDoors/RestroomBoothDoor.h"

ARestroomBoothDoor::ARestroomBoothDoor()
{
	BoxComp->SetBoxExtent(FVector(25.f, 10.f, 53.f));
	BoxComp->SetRelativeLocation(FVector(0.f, 0.f, 53.f), false);
}

void ARestroomBoothDoor::BeginPlay()
{
	Super::BeginPlay();

	TargetYaw = InitialYaw - MovableYaw;
}
