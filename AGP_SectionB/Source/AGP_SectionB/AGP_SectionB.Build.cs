// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AGP_SectionB : ModuleRules
{
	public AGP_SectionB(ReadOnlyTargetRules Target) : base(Target)
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
			"AGP_SectionB",
			"AGP_SectionB/Variant_Platforming",
			"AGP_SectionB/Variant_Platforming/Animation",
			"AGP_SectionB/Variant_Combat",
			"AGP_SectionB/Variant_Combat/AI",
			"AGP_SectionB/Variant_Combat/Animation",
			"AGP_SectionB/Variant_Combat/Gameplay",
			"AGP_SectionB/Variant_Combat/Interfaces",
			"AGP_SectionB/Variant_Combat/UI",
			"AGP_SectionB/Variant_SideScrolling",
			"AGP_SectionB/Variant_SideScrolling/AI",
			"AGP_SectionB/Variant_SideScrolling/Gameplay",
			"AGP_SectionB/Variant_SideScrolling/Interfaces",
			"AGP_SectionB/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
