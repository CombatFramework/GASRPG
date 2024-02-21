// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.


#include "Attributes/TestAbilitySetAttributes.h"
#include "Net/UnrealNetwork.h"

// Sets default values
UTestAbilitySetAttributes::UTestAbilitySetAttributes()
{
}

void UTestAbilitySetAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);
}

void UTestAbilitySetAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FGSCAttributeSetExecutionData ExecutionData;
    GetExecutionDataFromMod(Data, ExecutionData);
}

void UTestAbilitySetAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
    DOREPLIFETIME_CONDITION_NOTIFY(UTestAbilitySetAttributes, TestAbilitySet_01, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTestAbilitySetAttributes, TestAbilitySet_02, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UTestAbilitySetAttributes, TestAbilitySet_03, COND_None, REPNOTIFY_Always);
}

void UTestAbilitySetAttributes::OnRep_TestAbilitySet_01(const FGameplayAttributeData& OldTestAbilitySet_01)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAbilitySetAttributes, TestAbilitySet_01, OldTestAbilitySet_01);
}

void UTestAbilitySetAttributes::OnRep_TestAbilitySet_02(const FGameplayAttributeData& OldTestAbilitySet_02)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAbilitySetAttributes, TestAbilitySet_02, OldTestAbilitySet_02);
}

void UTestAbilitySetAttributes::OnRep_TestAbilitySet_03(const FGameplayAttributeData& OldTestAbilitySet_03)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAbilitySetAttributes, TestAbilitySet_03, OldTestAbilitySet_03);
}
