// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Materia : ModuleRules
{
	public Materia(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PublicDependencyModuleNames.AddRange(new string[] { "Sockets", "Networking" });

        PublicDependencyModuleNames.AddRange(new string[] { "zlib", "UElibPNG", "UElibJPG", "ImageWrapper" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Json", "JsonUtilities" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
