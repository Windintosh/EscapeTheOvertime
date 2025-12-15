#include "MainClass/Objects/Props/KeycardReader/KeycardReader.h"
#include "EscapeTheOvertimeCharacter.h"
#include "ElevatorDoor.h"
#include "Kismet/GameplayStatics.h"

AKeycardReader::AKeycardReader()
{

}

void AKeycardReader::ActivateProp(AActor* Activator)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(Activator);
	UE_LOG(LogTemp, Warning, TEXT("Keycard reader activated"));
	if (!PlayerCharacter) return;
	if (PlayerCharacter->bHasKeycard)
	{
		TArray<AActor*> EVDoors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AElevatorDoor::StaticClass(), EVDoors);
		for (AActor* EVDoor : EVDoors)
		{
			AElevatorDoor* OpenableDoor = Cast<AElevatorDoor>(EVDoor);
			if (OpenableDoor)
			{
				OpenableDoor->OpenDoor();
				UE_LOG(LogTemp, Warning, TEXT("Elevator Door is opened"));
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Elevator Door is not opened"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Get a keycard to open elevator!"));
	}
}
