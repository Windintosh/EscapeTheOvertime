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
			"Slate",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"EscapeTheOvertime",
			"EscapeTheOvertime/MainClass",
            "EscapeTheOvertime/MainClass/Objects",
            "EscapeTheOvertime/MainClass/Objects/Doors",
            "EscapeTheOvertime/MainClass/Objects/Doors/CabinetDoor",
            "EscapeTheOvertime/MainClass/Objects/Doors/ElevatorDoors",
            "EscapeTheOvertime/MainClass/Objects/Doors/MainDoors",
            "EscapeTheOvertime/MainClass/Objects/Doors/RestroomDoors",
            "EscapeTheOvertime/MainClass/Objects/Doors/LockerDoors",
            "EscapeTheOvertime/MainClass/Objects/Doors/SinkDoors",
            "EscapeTheOvertime/MainClass/Characters",
            "EscapeTheOvertime/Variant_Horror",
			"EscapeTheOvertime/Variant_Horror/UI",
			"EscapeTheOvertime/Variant_Shooter",
			"EscapeTheOvertime/Variant_Shooter/AI",
			"EscapeTheOvertime/Variant_Shooter/UI",
			"EscapeTheOvertime/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
