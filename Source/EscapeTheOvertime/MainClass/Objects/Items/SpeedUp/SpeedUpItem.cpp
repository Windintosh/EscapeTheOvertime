#include "MainClass/Objects/Items/SpeedUp/SpeedUpItem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EscapeTheOvertimeCharacter.h"
#include "HorrorCharacter.h"

ASpeedUpItem::ASpeedUpItem()
{
	ItemType = TEXT("SpeedUpItem");
	ItemIndex = 2;
}

void ASpeedUpItem::ActivateItem(AActor* Activator)
{
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(Activator);
	if (!PlayerCharacter) return;

	//PlayerCharacter->bIsSpedUp = true;
	//PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed * 2;
	//PlayerCharacter->SpeedUpItem.RemainingTime = PlayerCharacter->SpeedUpItem.Duration;
	PlayerCharacter->ActivateSpeedUp();

	DestroyItem();
}
