// Fill out your copyright notice in the Description page of Project Settings.


#include "MainClass/Objects/Doors/ElevatorDoors/ElevatorDoor.h"

// Sets default values
AElevatorDoor::AElevatorDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//75
}

// Called when the game starts or when spawned
void AElevatorDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElevatorDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

