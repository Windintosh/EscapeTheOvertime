#include "MainClass/Objects/Items/Distraction/DistractionItem.h"
#include "EscapeTheOvertimeCharacter.h"

ADistractionItem::ADistractionItem()
{
	ItemType = TEXT("DistractionItem");
	ItemIndex = 3;
}

void ADistractionItem::ActivateItem(AActor* Activator)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(Activator);
	if (!PlayerCharacter) return;

	//distract AI or obtain it for use

	DestroyItem();
}
