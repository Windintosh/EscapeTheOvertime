#include "MainClass/Objects/Items/Heal/HealItem.h"
#include "HorrorCharacter.h"

AHealItem::AHealItem()
{
	ItemType = TEXT("HealItem");
	ItemIndex = 4;
}

void AHealItem::ActivateItem(AActor* Activator)
{
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(Activator);
	if (!PlayerCharacter) return;

	PlayerCharacter->Heal(50.0f);

	DestroyItem();
}