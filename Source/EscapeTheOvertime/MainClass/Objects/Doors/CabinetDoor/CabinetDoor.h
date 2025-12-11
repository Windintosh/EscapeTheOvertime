#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CabinetDoor.generated.h"

UCLASS()
class ESCAPETHEOVERTIME_API ACabinetDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACabinetDoor();

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Animation")
	UTimelineComponent* CabTimeline;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UCurveFloat* CabDoorCurve;

	float InitialXValue;

	const float MovableXValue = 40.f;

	float TargetXValue;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void UpdateCabMovement(float Value);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
