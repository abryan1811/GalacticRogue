// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GalacticRogue : ModuleRules
{
	public GalacticRogue(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
