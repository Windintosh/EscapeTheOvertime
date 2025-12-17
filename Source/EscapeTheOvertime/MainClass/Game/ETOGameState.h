#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/GameState.h"
#include "GameFramework/GameStateBase.h"
#include "ItemSpawner.h"
#include "ETOGameState.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPETHEOVERTIME_API AETOGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void SpawnRandomItems();

	const int32 MinItemIndex = 0;
	const int32 MaxItemIndex = 3;

	bool bIsKeycardSpawned = false;
};
