#include "MainClass/Objects/Items/Distraction/ThrownItem.h"
#include "Boss/Boss.h"
#include "HorrorCharacter.h"
#include "Kismet/GameplayStatics.h"

AThrownItem::AThrownItem()
{
	ItemType = TEXT("ThrownItem");
	ItemIndex = 999;
}

void AThrownItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AHorrorCharacter* GameCharacter = Cast<AHorrorCharacter>(PlayerCharacter);


	GameCharacter->MakeNoise(5.f, GetActorLocation()); //Make noise at overlapped point, add condition(if boss) if glitches

	ActivateItem(OtherActor); //do sth to boss
}

void AThrownItem::ActivateItem(AActor* Activator)
{
	ABoss* Boss = Cast<ABoss>(Activator);
	if (!Boss) return;

	//Make boss stunned or sth

}
