#include "MainClass/Game/ETOGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AETOGameState::BeginPlay()
{
	Super::BeginPlay();

	SpawnRandomItems();

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AETOGameState::SpawnRandomItems()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	TArray<AActor*> ItemSpots;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawner::StaticClass(), ItemSpots);
	UE_LOG(LogTemp, Display, TEXT("%d Itemspots Spawned"), ItemSpots.Num());
	//for (AActor* ItemSpot : ItemSpots)
	//{
	//	int32 RandIndex = FMath::RandRange(MinItemIndex, MaxItemIndex);
	//	AItemSpawner* SpawnPoint = Cast<AItemSpawner>(ItemSpot);
	//	if (SpawnPoint)
	//	{
	//		SpawnPoint->SpawnItem(RandIndex);
	//		if (SpawnPoint->ItemIndex == 0)
	//		{
	//			break;
	//		}
	//	}
	//}
	int32 index = 0;
	int32 loopcount = 0;
	for (index = 0; index < ItemSpots.Num(); index++)
	{
		int32 RandIndex = FMath::RandRange(MinItemIndex, MaxItemIndex);
		AItemSpawner* SpawnPoint = Cast<AItemSpawner>(ItemSpots[index]);
		loopcount++;
		if (SpawnPoint)
		{
			SpawnPoint->SpawnItem(RandIndex);
			UE_LOG(LogTemp, Display, TEXT("%s Spawned(Index: %d) at point %d"), *SpawnPoint->ItemType.ToString(), SpawnPoint->ItemIndex, index);
			if (SpawnPoint->ItemIndex == 0) //if keycard, break the loop
			{
				bIsKeycardSpawned = true;
				UE_LOG(LogTemp, Display, TEXT("Keycard Spawned; doing next loop"));
				break;
			}
		}
		if (index == ItemSpots.Num() - 1) //if no keycard has spawned, do it again
		{
			index = -1;
			loopcount = 0;
			UE_LOG(LogTemp, Display, TEXT("No Keycard; retrying"));
			TArray<AActor*> DestroyedItems;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemBase::StaticClass(), DestroyedItems);
			for (AActor* DItem : DestroyedItems)
			{
				AItemBase* NoMore = Cast<AItemBase>(DItem);
				if (NoMore)
				{	
					NoMore->DestroyItem();
				}
			}
			//continue;
		}
	}
	for (loopcount; loopcount < ItemSpots.Num(); loopcount++) //spawn for the rest of the loop
	{
		int32 RandIndex = FMath::RandRange(MinItemIndex+1, MaxItemIndex); //no keycard
		AItemSpawner* SpawnPoint = Cast<AItemSpawner>(ItemSpots[loopcount]);
		if (SpawnPoint)
		{
			SpawnPoint->SpawnItem(RandIndex);
			UE_LOG(LogTemp, Display, TEXT("%s Spawned(Index: %d) at point %d"), *SpawnPoint->ItemType.ToString(), SpawnPoint->ItemIndex, loopcount);
		}
	}
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}
