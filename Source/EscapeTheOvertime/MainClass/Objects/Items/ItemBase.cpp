#include "MainClass/Objects/Items/ItemBase.h"
#include "EscapeTheOvertimeCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->InitSphereRadius(100.0f);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnItemEndOverlap);



}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(OtherActor);
	if (!PlayerCharacter) return;

	UE_LOG(LogTemp, Warning, TEXT("Player just touched an item %s"), *GetName());
	ActivateItem(OtherActor);
}

void AItemBase::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AItemBase::ActivateItem(AActor* Activator)
{

}

FName AItemBase::GetItemType() const
{
	return ItemType;
}

int32 AItemBase::GetItemIndex() const
{
	return ItemIndex;
}

void AItemBase::DestroyItem()
{
	Destroy();
}

