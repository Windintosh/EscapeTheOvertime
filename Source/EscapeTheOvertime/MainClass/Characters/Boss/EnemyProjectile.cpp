#include "MainClass/Characters/Boss/EnemyProjectile.h"
#include "Variant_Horror/HorrorCharacter.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	//SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(Collision);
	//Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->InitSphereRadius(40.0f);
	
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnPlayerOverlap);
	
}

void AEnemyProjectile::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHorrorCharacter* PlayerCharacter = Cast<AHorrorCharacter>(OtherActor);
	if (!PlayerCharacter)
	{
		DestroyProjectile();
		return;
	}

	PlayerCharacter->DamagePlayer(40.f);

	DestroyProjectile();
}

void AEnemyProjectile::DestroyProjectile()
{
	UE_LOG(LogTemp, Display, TEXT("%s is destroyed."), *GetName());
	Destroy();
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(NewLocation);
}

