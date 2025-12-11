// Fill out your copyright notice in the Description page of Project Settings.


#include "MainClass/Objects/CabinetDoor/CabinetDoor.h"

// Sets default values
ACabinetDoor::ACabinetDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACabinetDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACabinetDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

