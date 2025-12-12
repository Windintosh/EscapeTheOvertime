#include "MainClass/Objects/Doors/ElevatorDoors/ElevatorDoor.h"
#include "EscapeTheOvertimeCharacter.h"

// Sets default values
AElevatorDoor::AElevatorDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp->SetBoxExtent(FVector(70.f, 35.f, 147.f));
	BoxComp->SetRelativeLocation(FVector(0.f, 0.f, 147.f), false);

	MovableValue = 75.f;
}

// Called when the game starts or when spawned
void AElevatorDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialMoveValue = MeshComp->GetRelativeLocation().Y;
	TargetMoveValue = InitialMoveValue + MovableValue;//75
}

void AElevatorDoor::UpdateSDMovement(float Value)
{
	float NewLocation = FMath::Lerp(InitialMoveValue, TargetMoveValue, Value);
	float InitialXValue = MeshComp->GetRelativeLocation().X;
	float InitialZValue = MeshComp->GetRelativeLocation().Z;
	MeshComp->SetRelativeLocation(FVector(InitialXValue, NewLocation, InitialZValue), false);
}

// Called every frame
void AElevatorDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

