using UnrealBuildTool;

public class GASCompanionTestUtils : ModuleRules
{
    public GASCompanionTestUtils(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "EnhancedInput",
                "FunctionalTesting",
                "GameplayAbilities",
                "Slate",
                "SlateCore",
            }
        );
        
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "UnrealEd"
                }
            );
        }
    }
}