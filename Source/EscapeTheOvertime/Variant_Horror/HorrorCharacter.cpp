#include "Variant_Horror/HorrorCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

AHorrorCharacter::AHorrorCharacter()
{
	// create the spotlight
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(GetFirstPersonCameraComponent());

	SpotLight->SetRelativeLocationAndRotation(FVector(30.0f, 17.5f, -5.0f), FRotator(-18.6f, -1.3f, 5.26f));
	SpotLight->Intensity = 0.5;
	SpotLight->SetIntensityUnits(ELightUnits::Lumens);
	SpotLight->AttenuationRadius = 1050.0f;
	SpotLight->InnerConeAngle = 18.7f;
	SpotLight->OuterConeAngle = 45.24f;

	// 기본 HP값 설정
	MaxHP = 100.0f;
	CurrentHP = MaxHP;
	bIsDead = false;

	// 생성자에서 사운드 큐 파일 찾아서 로드하기 (Hard Loading)
	static ConstructorHelpers::FObjectFinder<USoundBase> DamageSoundAsset(TEXT("/Game/EscapeTheOvertime/07_FX/SFX/CinematicSound/Ugh_Cue.Ugh_Cue"));

	if (DamageSoundAsset.Succeeded())
	{
		DamageSound = DamageSoundAsset.Object;
	}
}

void AHorrorCharacter::BeginPlay()
{
	Super::BeginPlay();

	// initialize sprint meter to max
	SprintMeter = SprintTime;

	// 게임 시작 시 HP 초기화
	CurrentHP = MaxHP;
	bIsDead = false;

	// UI가 초기 상태(100%)를 그릴 수 있도록 방송
	if (MaxHP > 0.0f)
	{
		OnHealthChanged.Broadcast(CurrentHP / MaxHP);
	}

	// Initialize the walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// start the sprint tick timer
	GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &AHorrorCharacter::SprintFixedTick, SprintFixedTickTime, true);
}

void AHorrorCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the sprint timer
	GetWorld()->GetTimerManager().ClearTimer(SprintTimer);
}

// 보스가 때리면 자동 호출됨. 맞았을 때만 UI 업데이트 방송을 함.
float AHorrorCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 이미 죽었거나 데미지가 없으면 무시
	if (bIsDead || DamageAmount <= 0.0f)
	{
		return 0.0f;
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// HP 감소 및 0 미만 방지
	CurrentHP = FMath::Clamp(CurrentHP - ActualDamage, 0.0f, MaxHP);

	//  맞았을 때만 UI에게 "나 체력 변했어!"라고 알림
	if (MaxHP > 0.0f)
	{
		OnHealthChanged.Broadcast(CurrentHP / MaxHP);
	}

	// 데미지를 입었으면 소리 재생 (2D)
	if (ActualDamage > 0.0f && DamageSound)
	{
		// 플레이어 본인의 소리이므로 위치값 없이 2D로 재생 (헤드셋 전체에서 들림)
		UGameplayStatics::PlaySound2D(this, DamageSound);
	}

	// 사망 체크
	if (CurrentHP <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}

// 사망 처리 (C++)
void AHorrorCharacter::OnDeath_Implementation()
{
	if (bIsDead) return;

	bIsDead = true;

	// 입력 막기 (플레이어 멈춤)
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	UE_LOG(LogTemp, Warning, TEXT("Player Died. Check Blueprint for Cinematic logic."));

	// 실제 시네마틱 재생은 블루프린트의 'Event On Death' 노드에서 처리
}

void AHorrorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	{
		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			// Sprinting
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHorrorCharacter::DoStartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHorrorCharacter::DoEndSprint);
		}
	}
}

void AHorrorCharacter::DoStartSprint()
{
	// 죽었으면 달리기 불가
	if (bIsDead) return;

	// set the sprinting flag
	bSprinting = true;

	// are we out of recovery mode?
	if (!bRecovering)
	{
		// set the sprint walk speed
		if(!bIsSpedUp)
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		else
			GetCharacterMovement()->MaxWalkSpeed = IncreasedSpeed;
		// call the sprint state changed delegate
		OnSprintStateChanged.Broadcast(true);
	}

}

void AHorrorCharacter::DoEndSprint()
{
	// set the sprinting flag
	bSprinting = false;

	// are we out of recovery mode?
	if (!bRecovering)
	{
		// set the default walk speed
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		// call the sprint state changed delegate
		OnSprintStateChanged.Broadcast(false);
	}
}

void AHorrorCharacter::SprintFixedTick()
{
	// 죽었으면 스태미나 로직 중단
	if (bIsDead) return;

	// are we out of recovery, still have stamina and are moving faster than our walk speed?
	if (bSprinting && !bRecovering && GetVelocity().Length() > WalkSpeed)
	{

		// do we still have meter to burn?
		if (SprintMeter > 0.0f)
		{
			// update the sprint meter
			if(!bIsSpedUp)
				SprintMeter = FMath::Max(SprintMeter - SprintFixedTickTime, 0.0f);


			// have we run out of stamina?
			if (SprintMeter <= 0.0f)
			{
				// raise the recovering flag
				bRecovering = true;

				// set the recovering walk speed
				GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;
			}
		}

	}
	else {

		// recover stamina
		SprintMeter = FMath::Min(SprintMeter + SprintFixedTickTime, SprintTime);

		if (SprintMeter >= SprintTime)
		{
			// lower the recovering flag
			bRecovering = false;

			// set the walk or sprint speed depending on whether the sprint button is down
			if (!bIsSpedUp)
				GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
			else
				GetCharacterMovement()->MaxWalkSpeed = bSprinting ? IncreasedSpeed : SprintSpeed;	

			// update the sprint state depending on whether the button is down or not
			OnSprintStateChanged.Broadcast(bSprinting);
		}

	}

	// broadcast the sprint meter updated delegate
	OnSprintMeterUpdated.Broadcast(SprintMeter / SprintTime);
}