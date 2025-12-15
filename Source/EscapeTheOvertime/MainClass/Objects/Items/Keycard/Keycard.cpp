#include "MainClass/Objects/Items/Keycard/Keycard.h"
#include "Keycard.h"
#include "EscapeTheOvertimeCharacter.h"

AKeycard::AKeycard()
{
	ItemType = TEXT("Keycard");
	ItemIndex = 0;
}

void AKeycard::ActivateItem(AActor* Activator)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(Activator);
	if (!PlayerCharacter) return;
	PlayerCharacter->bHasKeycard = true;
	DestroyItem();
}
