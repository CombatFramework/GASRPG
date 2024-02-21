// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.


#include "Attributes/TestAbilitySetAttributes_03.h"
#include "Net/UnrealNetwork.h"

// Sets default values
UTestAbilitySetAttributes_03::UTestAbilitySetAttributes_03()
{
}

void UTestAbilitySetAttributes_03::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    // This is called whenever attributes change, so for max attributes we want to scale the current totals to match
    Super::PreAttributeChange(Attribute, NewValue);
}

void UTestAbilitySetAttributes_03::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FGSCAttributeSetExecutionData ExecutionData;
    GetExecutionDataFromMod(Data, ExecutionData);
}

void UTestAbilitySetAttributes_03::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
    DOREPLIFETIME_CONDITION_NOTIFY(UTestAbilitySetAttributes_03, TestAbilitySet_07, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTestAbilitySetAttributes_03, TestAbilitySet_08, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTestAbilitySetAttributes_03, TestAbilitySet_09, COND_None, REPNOTIFY_Always);
}

void UTestAbilitySetAttributes_03::OnRep_TestAbilitySet_07(const FGameplayAttributeData& OldTestAbilitySet_07)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAbilitySetAttributes_03, TestAbilitySet_07, OldTestAbilitySet_07);
}

void UTestAbilitySetAttributes_03::OnRep_TestAbilitySet_08(const FGameplayAttributeData& OldTestAbilitySet_08)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAbilitySetAttributes_03, TestAbilitySet_08, OldTestAbilitySet_08);
}

void UTestAbilitySetAttributes_03::OnRep_TestAbilitySet_09(const FGameplayAttributeData& OldTestAbilitySet_09)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAbilitySetAttributes_03, TestAbilitySet_09, OldTestAbilitySet_09);
}
