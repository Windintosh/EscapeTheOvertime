#include "MainClass/Game/AnimNotifies/AnimNotify_ThrowDO.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EscapeTheOvertimeCharacter.h"
#include "ThrownItem.h"


void UAnimNotify_ThrowDO::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	AEscapeTheOvertimeCharacter* PlayerCharacter = Cast<AEscapeTheOvertimeCharacter>(MeshComp->GetOwner());
	//MeshComp->GetSocketByName(TEXT("ItemSocket"));

	if (PlayerCharacter && ThrownItemClass)
	{
		FVector SpawnLocation = MeshComp->GetSocketLocation(TEXT("ItemSocket"));
		FRotator SpawnRotation = PlayerCharacter->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PlayerCharacter;
		SpawnParams.Instigator = PlayerCharacter;

		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//spawn item to throw
		AThrownItem* ThrownItem = GetWorld()->SpawnActor<AThrownItem>(ThrownItemClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (ThrownItem) 
		{
			PlayerCharacter->DistractionItemQuantity--;
			UProjectileMovementComponent* ProjectileComp = ThrownItem->FindComponentByClass<UProjectileMovementComponent>();
			if (ProjectileComp)
			{
				// initial velocity set as player's direction
				FVector LaunchDirection = PlayerCharacter->GetActorForwardVector();
				ProjectileComp->Velocity = LaunchDirection * ProjectileComp->InitialSpeed;
			}
			ThrownItem->SetActorEnableCollision(false);
		}
	}
}
