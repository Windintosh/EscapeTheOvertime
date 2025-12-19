#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "EscapeTheOvertimeCharacter.h"
#include "HorrorCharacter.generated.h"

class USpotLightComponent;
class UInputAction;
class USoundBase;
class UTexture2D;

// 스태미나 관련 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSprintMeterDelegate, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSprintStateChangedDelegate, bool, bSprinting);

// HP 변경 알림용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedDelegate, float, HPPercent);

// [변경] 아이템 획득 알림용 델리게이트 (파라미터 3개: 이름, 설명, 아이콘)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemPickedUpDelegate, FText, Name, FText, Desc, UTexture2D*, Icon);

/**
 * Simple first person horror character
 * Provides stamina-based sprinting and Health System
 */
UCLASS(abstract)
class ESCAPETHEOVERTIME_API AHorrorCharacter : public AEscapeTheOvertimeCharacter
{
	GENERATED_BODY()

	/** Player light source */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLight;

protected:

	/** Fire weapon input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	/** If true, we're sprinting */
	bool bSprinting = false;

	/** If true, we're recovering stamina */
	bool bRecovering = false;

	/** Default walk speed when not sprinting or recovering */
	UPROPERTY(EditAnywhere, Category = "Walk")
	float WalkSpeed = 250.0f;

	/** Time interval for sprinting stamina ticks */
	UPROPERTY(EditAnywhere, Category = "Sprint", meta = (ClampMin = 0, ClampMax = 1, Units = "s"))
	float SprintFixedTickTime = 0.03333f;

	/** Sprint stamina amount. Maxes at SprintTime */
	float SprintMeter = 0.0f;

	/** How long we can sprint for, in seconds */
	UPROPERTY(EditAnywhere, Category = "Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float SprintTime = 5.0f;

	/** Walk speed while sprinting */
	UPROPERTY(EditAnywhere, Category = "Sprint", meta = (ClampMin = 0, Units = "cm/s"))
	float SprintSpeed = 600.0f;

	/** Walk speed while recovering stamina */
	UPROPERTY(EditAnywhere, Category = "Recovery", meta = (ClampMin = 0, Units = "cm/s"))
	float RecoveringWalkSpeed = 150.0f;

	/** Time it takes for the sprint meter to recover */
	UPROPERTY(EditAnywhere, Category = "Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RecoveryTime = 0.0f;

	/** Sprint tick timer */
	FTimerHandle SprintTimer;

	/* ================================================== */
	/* HP & Death System                         */
	/* ================================================== */

	/** Max HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHP = 100.0f;

	/** Current HP */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHP;

	/** Is Dead? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsDead = false;

	/** 피격 시 재생할 사운드 (Sound Cue) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* DamageSound;

public:

	/** Delegate called when the sprint meter should be updated */
	FUpdateSprintMeterDelegate OnSprintMeterUpdated;

	/** Delegate called when we start and stop sprinting */
	FSprintStateChangedDelegate OnSprintStateChanged;

	// 맞았을 때만 UI를 업데이트하기 위한 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChangedDelegate OnHealthChanged;

	// [변경] 아이템 획득 시 방송(Broadcast)할 이벤트 디스패처
	// 블루프린트에서 이 이벤트를 Assign(바인딩)하여 UI를 띄웁니다.
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnItemPickedUpDelegate OnItemPickedUp;

	// 외부에서 HP 정보를 가져오기 위한 유틸리티 함수
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHP() const { return CurrentHP; }

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(Category = "Health")
	void Heal(float Amount);

	// 앉기 입력 액션 (에디터에서 할당)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* CrouchAction;

protected:

	/** Constructor */
	AHorrorCharacter();

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Gameplay cleanup */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// 언리얼 엔진 기본 피격 함수 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 사망 시 실행할 이벤트 (블루프린트에서 시네마틱 연결용)
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDeath();
	virtual void OnDeath_Implementation();

	// 앉기 시작/종료 함수
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);

protected:

	/** Starts sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartSprint();

	/** Stops sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoEndSprint();

	/** Called while sprinting at a fixed time interval */
	void SprintFixedTick();
};