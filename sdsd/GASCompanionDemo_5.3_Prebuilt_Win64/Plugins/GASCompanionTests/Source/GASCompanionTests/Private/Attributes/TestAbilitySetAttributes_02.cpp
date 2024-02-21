// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.


#include "Attributes/TestAbilitySetAttributes_02.h"
#include "Net/UnrealNetwork.h"

// Sets default values
UTestAbilitySetAttributes_02::UTestAbilitySetAttributes_02()
{
}

void UTestAbilitySetAttributes_02::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    // This is called whenever attributes change, so for max attributes we want to scale the current totals to match
    Super::PreAttributeChange(Attribute, NewValue);
}

void UTestAbilitySetAttributes_02::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FGSCAttributeSetExecutionData ExecutionData;
    GetExecutionDataFromMod(Data, ExecutionData);
}

void UTestAbilitySetAttributes_02::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
    DOREPLIFETIME_CONDITION_NOTIFY(UTestAbilitySetAttributes_02, TestAbilitySet_04, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTestAbilitySetAttributes_02, TestAbilitySet_05, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTestAbilitySetAttributes_02, TestAbilitySet_06, COND_None, REPNOTIFY_Always);
}

void UTestAbilitySetAttributes_02::OnRep_TestAbilitySet_04(const FGameplayAttributeData& OldTestAbilitySet_04)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAbilitySetAttributes_02, TestAbilitySet_04, OldTestAbilitySet_04);
}

void UTestAbilitySetAttributes_02::OnRep_TestAbilitySet_05(const FGameplayAttributeData& OldTestAbilitySet_05)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAbilitySetAttributes_02, TestAbilitySet_05, OldTestAbilitySet_05);
}

void UTestAbilitySetAttributes_02::OnRep_TestAbilitySet_06(const FGameplayAttributeData& OldTestAbilitySet_06)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAbilitySetAttributes_02, TestAbilitySet_06, OldTestAbilitySet_06);
}
