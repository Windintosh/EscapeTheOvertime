#pragma once

#include "CoreMinimal.h"
#include "Boss.h"
#include "Dancer.generated.h"

UCLASS()
class ESCAPETHEOVERTIME_API ADancer : public ABoss
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADancer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
