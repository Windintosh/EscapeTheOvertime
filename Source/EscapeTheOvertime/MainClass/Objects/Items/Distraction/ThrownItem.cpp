#include "MainClass/Objects/Items/Distraction/ThrownItem.h"
#include "Boss/Boss.h"
#include "HorrorCharacter.h"
#include "Kismet/GameplayStatics.h"

AThrownItem::AThrownItem()
{
	ItemType = TEXT("ThrownItem");
	ItemIndex = 999;
	/*
	if (StaticMesh) StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Collision) SetRootComponent(Collision);

	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetNotifyRigidBodyCollision(true);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Collision->SetUseCCD(true);
	

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	GeometryCollectionComponent->SetupAttachment(RootComponent);

	// Set the initial Geometry Collection asset
	if (RestCollectionAsset)
	{
		GeometryCollectionComponent->SetRestCollection(RestCollectionAsset);
		GeometryCollectionComponent->SetSimulatePhysics(false);
		GeometryCollectionComponent->SetNotifyRigidBodyCollision(true); // 
		GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 
	}

	// Ensure the physics state is properly created at runtime
	GeometryCollectionComponent->RecreatePhysicsState();

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovement->UpdatedComponent = RootComponent; // Tell the movement component to update the collision component's position
	ProjectileMovement->InitialSpeed = 1000.f; // Set initial speed
	ProjectileMovement->MaxSpeed = 2000.f; // Set max speed
	ProjectileMovement->bRotationFollowsVelocity = true; // Projectile rotation matches its direction
	ProjectileMovement->bSweepCollision = true; //Checks collision every frame
	ProjectileMovement->bShouldBounce = false; // Enable simple bouncing
	//ProjectileMovement->Bounciness = 0.0f;
	ProjectileMovement->ProjectileGravityScale = 1.0f; // Standard gravity
	*/

	// 부모에서 설정했어도 안전을 위해 확인
	if (Collision) SetRootComponent(Collision);

	// Static Mesh 충돌 끄기 (확실하게)
	if (StaticMesh)
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// [핵심] Collision 설정 강화
	if (Collision)
	{
		Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Collision->SetNotifyRigidBodyCollision(true);

		// [수정] 벽(Static)과 물체(Dynamic) 모두 Block
		Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // 던지는 사람은 무시
		Collision->SetUseCCD(true);
	}

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	GeometryCollectionComponent->SetupAttachment(RootComponent);

	if (RestCollectionAsset)
	{
		GeometryCollectionComponent->SetRestCollection(RestCollectionAsset);
		GeometryCollectionComponent->SetSimulatePhysics(false);
		GeometryCollectionComponent->SetNotifyRigidBodyCollision(false); // 날아갈 땐 꺼둠
		GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 발사체 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 1500.f; // 속도 조금 더 올림
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bSweepCollision = true;
	ProjectileMovement->bSimulationEnabled = true; // 이동 시뮬레이션 켜기
	ProjectileMovement->bInterpMovement = true;
	ProjectileMovement->bForceSubStepping = true; // 정확도 향상
	ProjectileMovement->bShouldBounce = false; // 튕기기 끔 -> 즉시 Stop 이벤트 발생
	ProjectileMovement->ProjectileGravityScale = 1.0f;

}

void AThrownItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//AHorrorCharacter* GameCharacter = Cast<AHorrorCharacter>(PlayerCharacter);

	////GeometryCollectionComponent->SetSimulatePhysics(true);
	//GameCharacter->MakeNoise(5.f, GetActorLocation()); //Make noise at overlapped point, add condition(if boss) if glitches

	//ActivateItem(OtherActor); //do sth to boss
}

void AThrownItem::OnItemHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Error, TEXT("OnHit Called! Hit Actor: %s"), *OtherActor->GetName());
	if ((OtherActor == NULL) || (OtherActor == this) || (OtherActor == GetInstigator()))
	{
		return;
	}
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AHorrorCharacter* GameCharacter = Cast<AHorrorCharacter>(PlayerCharacter);

	//GeometryCollectionComponent->SetSimulatePhysics(true);
	GameCharacter->MakeNoise(5.f, GetActorLocation()); //Make noise at overlapped point, add condition(if boss) if glitches

	ActivateItem(OtherActor); //do sth to boss

	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->ProjectileGravityScale = 0.0f; // disable gravity(optional)
	}
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (GeometryCollectionComponent)
	{
		// activate collision
		GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		// Simulate physics
		GeometryCollectionComponent->SetSimulatePhysics(true);

		// (Optional) add impulse to impact point for dramatic
		// GeometryCollectionComponent->AddRadialImpulse(Hit.ImpactPoint, 500.0f, 2000.0f, ERadialImpulseFalloff::RIF_Linear, true);
	}
	SetLifeSpan(5.0f); //destroy after
}

void AThrownItem::ActivateItem(AActor* Activator)
{
	ABoss* Boss = Cast<ABoss>(Activator);
	if (!Boss) return;

	//Make boss stunned or sth
	IBossHit::Execute_Interact(Boss, this); //stun Boss
}

void AThrownItem::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugSphere(GetWorld(), GetActorLocation(), Collision->GetScaledSphereRadius(), 12, FColor::Red, true, 5.0f);

	//Collision->OnComponentBeginOverlap.RemoveDynamic(this, &AItemBase::OnItemOverlap);
	Collision->OnComponentHit.AddDynamic(this, &AThrownItem::OnItemHit);

	//if (ProjectileMovement)
	//{
	//	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AThrownItem::OnProjectileStop);
	//}

	//if (ProjectileMovement->OnProjectileStop.IsAlreadyBound(this, &AThrownItem::OnProjectileStop))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("OnProjectileStop is bound correctly!"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("OnProjectileStop binding FAILED!"));
	//}

	//if (Collision->OnComponentHit.IsAlreadyBound(this, &AThrownItem::OnItemHit))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("OnItemHit is bound correctly!"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("OnItemHit binding FAILED!"));
	//}


	// 디버깅: 실제 반경이 몇인지 로그로 확인
	if (Collision)
	{
		float Radius = Collision->GetScaledSphereRadius();
		UE_LOG(LogTemp, Warning, TEXT("Current Sphere Radius: %f"), Radius);

		if (Radius <= 0.1f)
		{
			UE_LOG(LogTemp, Error, TEXT("CRITICAL ERROR: Radius is Zero! Force setting it."));
			Collision->SetSphereRadius(7.f);
			Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}


	// 1. [핵심] 충돌체(Sphere)가 있다면 설정을 강제 집행합니다.
	if (Collision)
	{
		// 혹시 모를 크기 0 방지
		Collision->SetSphereRadius(7.f);

		// 일단 "모든 것"과 충돌하게 설정 (Block)
		Collision->SetCollisionResponseToAllChannels(ECR_Block);

		// 던진 사람(Pawn)만 충돌 무시 (안 그러면 던지자마자 터짐)
		//Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		Collision->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // 카메라도 무시 추천

		// 움직일 때 충돌 감지(Sweep)를 위한 플래그 재확인
		Collision->SetNotifyRigidBodyCollision(true);
		Collision->SetGenerateOverlapEvents(true);
		if (GetInstigator())
		{
			Collision->IgnoreActorWhenMoving(GetInstigator(), true); // 이동 중 충돌 무시
			Collision->MoveIgnoreActors.Add(GetInstigator());
		}
	}

	// 2. Projectile Movement 연결 재확인
	if (ProjectileMovement)
	{
		// 움직이는 주체가 Root(Collision)인지 확실하게 지정
		ProjectileMovement->SetUpdatedComponent(Collision);

		// [중요] SweepCollision이 꺼져 있으면 벽을 통과합니다. 강제로 켭니다.
		ProjectileMovement->bSweepCollision = true;

		// 이벤트 바인딩
		ProjectileMovement->OnProjectileStop.AddDynamic(this, &AThrownItem::OnProjectileStop);
	}

	if (StaticMesh)
	{
		StaticMesh->SetVisibility(true);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌은 오직 Sphere가 담당
	}

	// 3. GC 물리 끄기 (날아가는 동안 방해 안 되게)
	if (GeometryCollectionComponent)
	{
		GeometryCollectionComponent->SetVisibility(false);
		GeometryCollectionComponent->SetSimulatePhysics(false);
		GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}




	// 3. ignore instigator(who threw)
	if (GetInstigator())
	{
		Collision->MoveIgnoreActors.Add(GetInstigator()); // ignore collision when moving
		// or
		Collision->IgnoreActorWhenMoving(GetInstigator(), true);
	}
}

void AThrownItem::OnProjectileStop(const FHitResult& ImpactResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile Stopped! Hit Actor: %s"), *ImpactResult.GetActor()->GetName());
	AActor* HitActor = ImpactResult.GetActor();
	if ((HitActor == NULL) || (HitActor == this) || (HitActor == GetInstigator()))
	{
		return;
	}
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AHorrorCharacter* GameCharacter = Cast<AHorrorCharacter>(PlayerCharacter);

	//GeometryCollectionComponent->SetSimulatePhysics(true);
	GameCharacter->MakeNoise(5.f, GetActorLocation()); //Make noise at overlapped point, add condition(if boss) if glitches

	ActivateItem(HitActor); //do sth to boss

	// 1. 둥둥 떠있게 될 Static Mesh를 숨깁니다.
	if (StaticMesh)
	{
		StaticMesh->SetVisibility(false);
	}

	// 2. 루트(Sphere)의 충돌을 끕니다. (더 이상 충돌 방지)
	if (Collision)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (GeometryCollectionComponent)
	{
		GeometryCollectionComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
		GeometryCollectionComponent->SetVisibility(true);
		GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GeometryCollectionComponent->SetSimulatePhysics(true);
		// 
		// GeometryCollectionComponent->AddRadialImpulse(ImpactResult.ImpactPoint, 500.0f, 2000.0f, ERadialImpulseFalloff::RIF_Linear, true);
		// 벽에 부딪힌 충격 방향으로 힘을 가해줍니다.
		FVector ImpulseDir = ProjectileMovement->Velocity.GetSafeNormal();
		GeometryCollectionComponent->AddImpulse(ImpulseDir * 5000.0f, NAME_None, true);

	}

	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
	}

	SetLifeSpan(5.0f); //
}

void AThrownItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Collision)
	{
		// 1. 크기가 0이 되지 않도록 강제 설정
		if (Collision->GetUnscaledSphereRadius() <= 0.0f)
		{
			Collision->SetSphereRadius(7.f); // 원하는 크기로 설정
		}

		// 2. 물리 상태 강제 재생성 (구체가 안 보일 때 특효약)
		// 이 함수는 "물리 엔진아, 나 여기 있어! 다시 그려줘"라고 명령하는 것입니다.
		Collision->RecreatePhysicsState();
	}

}
