// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Horror/HorrorPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "EscapeTheOvertimeCameraManager.h"
#include "HorrorCharacter.h"
#include "HorrorUI.h"
#include "EscapeTheOvertime.h"
#include "Widgets/Input/SVirtualJoystick.h"

AHorrorPlayerController::AHorrorPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AEscapeTheOvertimeCameraManager::StaticClass();

	// [Time System] Tick 활성화 및 변수 초기화
	PrimaryActorTick.bCanEverTick = true;
	CurrentHour = 21;   //  시작 시간
	CurrentMinute = 0.0f;
	TimeSpeed = 10.0f;  // 시간 흐름 속도 (조절 가능)
	bIsGameOver = false;
}

void AHorrorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		}
		else {

			UE_LOG(LogEscapeTheOvertime, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

// [Time System] 매 프레임 시간 계산 로직
void AHorrorPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 게임 오버 상태면 시간 멈춤
	if (bIsGameOver)
	{
		return;
	}

	// 1. 분 누적 (DeltaTime * 속도)
	CurrentMinute += DeltaTime * TimeSpeed;

	// 2. 분 -> 시간 변환
	if (CurrentMinute >= 60.0f)
	{
		CurrentMinute -= 60.0f;
		CurrentHour++;

		// 3. 자정(24시) 체크
		if (CurrentHour >= 24)
		{
			bIsGameOver = true;
			CurrentHour = 0; // 0시 표기 (선택사항)

			// 블루프린트로 이벤트 호출 (암전, 게임오버 UI 등 연출)
			OnTimeLimitReached();
		}
	}
}

void AHorrorPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// only spawn UI on local player controllers
	if (IsLocalPlayerController())
	{
		// set up the UI for the character
		if (AHorrorCharacter* HorrorCharacter = Cast<AHorrorCharacter>(aPawn))
		{
			// create the UI
			if (!HorrorUI)
			{
				HorrorUI = CreateWidget<UHorrorUI>(this, HorrorUIClass);
				HorrorUI->AddToViewport(0);
			}

			HorrorUI->SetupCharacter(HorrorCharacter);
		}
	}

}

void AHorrorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}