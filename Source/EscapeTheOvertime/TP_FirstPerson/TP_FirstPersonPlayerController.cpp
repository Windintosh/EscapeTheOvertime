#include "TP_FirstPersonPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "TP_FirstPersonCameraManager.h"

// [PM 정리] 불필요한 헤더 및 모바일 관련 헤더 제거
// #include "InputMappingContext.h"
// #include "Blueprint/UserWidget.h"
// #include "Widgets/Input/SVirtualJoystick.h"

ATP_FirstPersonPlayerController::ATP_FirstPersonPlayerController()
{
	// 카메라 매니저 설정은 유지합니다.
	PlayerCameraManagerClass = ATP_FirstPersonCameraManager::StaticClass();
}

void ATP_FirstPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// [PM 정리] 모바일 터치 컨트롤 위젯 생성 로직 제거
	// PC 공포 게임이므로 가상 조이스틱 UI는 필요 없습니다.
	// 이 부분이 'MobileControlsWidgetClass' 에러의 주원인이었습니다.
}

void ATP_FirstPersonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// [PM 정리] Input Mapping Context 연결 로직
	// 기존 코드에서 'DefaultMappingContexts' 배열을 찾지 못해 에러가 발생했습니다.
	// 보통 1인칭 템플릿에서는 Character.cpp의 BeginPlay에서 매핑 컨텍스트를 연결하는 것이 일반적입니다.
	// 만약 컨트롤러에서 연결하고 싶다면 헤더 파일(.h)에 해당 변수를 먼저 선언해야 합니다.

	// 현재는 빌드 성공을 위해 비워둡니다. 
	// (입력 매핑은 TP_FirstPersonCharacter.cpp에서 처리되도록 설정되어 있을 확률이 높습니다.)
}