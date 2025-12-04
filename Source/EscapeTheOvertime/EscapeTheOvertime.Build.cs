// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EscapeTheOvertime : ModuleRules
{
	public EscapeTheOvertime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"EscapeTheOvertime",
			"EscapeTheOvertime/Variant_Platforming",
			"EscapeTheOvertime/Variant_Platforming/Animation",
			"EscapeTheOvertime/Variant_Combat",
			"EscapeTheOvertime/Variant_Combat/AI",
			"EscapeTheOvertime/Variant_Combat/Animation",
			"EscapeTheOvertime/Variant_Combat/Gameplay",
			"EscapeTheOvertime/Variant_Combat/Interfaces",
			"EscapeTheOvertime/Variant_Combat/UI",
			"EscapeTheOvertime/Variant_SideScrolling",
			"EscapeTheOvertime/Variant_SideScrolling/AI",
			"EscapeTheOvertime/Variant_SideScrolling/Gameplay",
			"EscapeTheOvertime/Variant_SideScrolling/Interfaces",
			"EscapeTheOvertime/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
