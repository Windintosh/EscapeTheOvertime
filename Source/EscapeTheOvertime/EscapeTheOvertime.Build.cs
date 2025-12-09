// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EscapeTheOvertime : ModuleRules
{
    public EscapeTheOvertime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // 1. 필수 의존성 모듈 (1인칭, AI, UI, 입력)
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "AIModule",        // 부장님 AI 필수
			"NavigationSystem", // 네비게이션 메시 필수
			"UMG",             // UI 위젯 필수
			"Slate",
            "SlateCore"
        });

        // StateTree 관련 모듈은 만약 사용하지 않는다면 주석 처리해도 됩니다.
        // 혹시 남아있는 AI 코드에서 쓸 수 있어 남겨둡니다.
        PublicDependencyModuleNames.AddRange(new string[] {
            "StateTreeModule",
            "GameplayStateTreeModule",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // 2. [중요] 삭제한 Variant 폴더들의 경로를 모두 제거했습니다.
        // 프로젝트 루트 경로만 포함하면 엔진이 알아서 하위 폴더를 스캔합니다.
        PublicIncludePaths.AddRange(new string[] {
            "EscapeTheOvertime"
        });
    }
}