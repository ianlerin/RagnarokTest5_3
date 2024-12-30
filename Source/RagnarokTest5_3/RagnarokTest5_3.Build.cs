// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RagnarokTest5_3 : ModuleRules
{
	public RagnarokTest5_3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities",
                "GameplayTags",
                "GameplayTasks",  "UMG" });
	}
}
