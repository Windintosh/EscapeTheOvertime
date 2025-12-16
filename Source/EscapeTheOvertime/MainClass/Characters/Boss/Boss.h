#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss.generated.h" // generated 헤더는 항상 마지막에 포함되어야 합니다.

// 포워드 선언 (컴파일 속도 향상)
class ABossAIController;
class UAnimMontage;

UCLASS()
class ESCAPETHEOVERTIME_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* ================================================== */
	/* Combat System (New)                               */
	/* 이동은 AIController가 하므로, 공격 관련 기능만 추가  */
	/* ================================================== */

	/** 에디터에서 할당할 공격 애니메이션 몽타주 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	/** Behavior Tree(Task)에서 호출할 공격 함수 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();

private:
	/** 몽타주 재생이 끝났을 때 호출되는 콜백 함수 */
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/* [삭제됨]
	   기존의 SetPatrolPoints, MoveToNextPatrolPoint, IsCloseToTarget 등
	   직접 이동을 계산하던 변수와 함수들은 AIController 시스템과 충돌하므로 제거했습니다.
	*/
};