// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Treasure_Hunter : ModuleRules
{
	public Treasure_Hunter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
