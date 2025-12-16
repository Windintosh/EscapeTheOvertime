#include "MainClass/Objects/Items/Muffle/MuffleItem.h"
#include "EscapeTheOvertimeCharacter.h"

AMuffleItem::AMuffleItem()
{
	ItemType = TEXT("MuffleItem");
	ItemIndex = 1;
}

void AMuffleItem::ActivateItem(AActor* Activator)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(Activator);
	if (!PlayerCharacter) return;

	//PlayerCharacter->bIsMuffled = true;
	//PlayerCharacter->MuffleItem.RemainingTime = PlayerCharacter->MuffleItem.Duration;
	PlayerCharacter->ActivateMuffle();

	DestroyItem();
}
