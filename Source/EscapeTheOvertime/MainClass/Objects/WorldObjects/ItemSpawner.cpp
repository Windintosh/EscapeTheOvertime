#include "MainClass/Objects/WorldObjects/ItemSpawner.h"
#include "Components/BoxComponent.h"
#include "Keycard.h"
#include "MuffleItem.h"
#include "SpeedUpItem.h"
#include "DistractionItem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	//BoxComp->SetMobility(EComponentMobility::Static);
	//BoxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	BoxComp->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnItem(FMath::RandRange(0, 3)); //need to move to GameInstance or sth
}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* AItemSpawner::SpawnItem(int32 Index)
{
	
	switch(Index)
	{
	case 0:
	{
		AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(KeycardClass, GetActorLocation(), GetActorRotation());
		return SpawnedItem;
	}
	case 1:
	{
		AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(MuffleItemClass, GetActorLocation(), GetActorRotation());
		return SpawnedItem;
	}
	case 2:
	{
		AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(SpeedUpItemClass, GetActorLocation(), GetActorRotation());
		return SpawnedItem;
	}
	case 3:
	{
		AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(DistractionItemClass, GetActorLocation(), GetActorRotation());
		return SpawnedItem;
	}
	default:
		return nullptr;
	}

}

