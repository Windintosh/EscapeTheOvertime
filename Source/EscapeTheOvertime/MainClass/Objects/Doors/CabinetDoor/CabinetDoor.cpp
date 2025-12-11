#include "CabinetDoor.h"
#include "UObject/ConstructorHelpers.h"
#include "EscapeTheOvertimeCharacter.h"


// Sets default values
ACabinetDoor::ACabinetDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp); // RootComponent = BoxComp;
	BoxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComp->SetBoxExtent(FVector(60.f, 60.f, 7.5f)); //make it half!

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComp->SetupAttachment(BoxComp);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CabinetMesh(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/File_Cabinets_02-Freepoly_org/MiniCabinets_Door5.MiniCabinets_Door5"));
	if (CabinetMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(CabinetMesh.Object);
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get StaticMesh for %s"), *GetName());
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> CabinetMaterial(TEXT("/Game/EscapeTheOvertime/00_Maps/UseAsset/File_Cabinets_02-Freepoly_org/21___Default.21___Default"));
	if (CabinetMaterial.Succeeded())
	{
		MeshComp->GetStaticMesh()->SetMaterial(0, CabinetMaterial.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Material for %s"), *GetName());
	}
	
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("UpdateCabMovement"));
	CabTimeline->AddInterpFloat(CabDoorCurve, TimelineCallback);
	

}

// Called when the game starts or when spawned
void ACabinetDoor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACabinetDoor::OnOverlapBegin);

	InitialXValue = MeshComp->GetRelativeLocation().X;
	TargetXValue = InitialXValue + MovableXValue;
}

void ACabinetDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//add this later, after making character
	

	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(OtherActor);
	if (PlayerCharacter && !bIsOpen)
	{
		//timeline failed-> fix it later
		//CabTimeline->PlayFromStart();
		UE_LOG(LogTemp, Warning, TEXT("Character is near Cabinet"));	
		UpdateCabMovement(1.0f);
		bIsOpen = true;
	}
}

void ACabinetDoor::UpdateCabMovement(float Value)
{
	float NewLocation = FMath::Lerp(InitialXValue, TargetXValue, Value);
	float InitialYValue = MeshComp->GetRelativeLocation().Y;
	float InitialZValue = MeshComp->GetRelativeLocation().Z;
	MeshComp->SetRelativeLocation(FVector(NewLocation, InitialYValue, InitialZValue), false);
}

// Called every frame
void ACabinetDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

