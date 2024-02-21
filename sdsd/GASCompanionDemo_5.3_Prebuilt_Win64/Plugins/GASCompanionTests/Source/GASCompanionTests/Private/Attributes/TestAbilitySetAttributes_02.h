// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "TestAbilitySetAttributes_02.generated.h"

UCLASS()
class UTestAbilitySetAttributes_02 : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	// Sets default values for this AttributeSet attributes
	UTestAbilitySetAttributes_02();

    // AttributeSet Overrides
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
        
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_TestAbilitySet_04)
    FGameplayAttributeData TestAbilitySet_04 = 400.f;
    ATTRIBUTE_ACCESSORS(UTestAbilitySetAttributes_02, TestAbilitySet_04)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_TestAbilitySet_05)
    FGameplayAttributeData TestAbilitySet_05 = 500.f;
    ATTRIBUTE_ACCESSORS(UTestAbilitySetAttributes_02, TestAbilitySet_05)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_TestAbilitySet_06)
    FGameplayAttributeData TestAbilitySet_06 = 600.f;
    ATTRIBUTE_ACCESSORS(UTestAbilitySetAttributes_02, TestAbilitySet_06)    

protected:
    
    UFUNCTION()
    virtual void OnRep_TestAbilitySet_04(const FGameplayAttributeData& OldTestAbilitySet_04);

    UFUNCTION()
    virtual void OnRep_TestAbilitySet_05(const FGameplayAttributeData& OldTestAbilitySet_05);

    UFUNCTION()
    virtual void OnRep_TestAbilitySet_06(const FGameplayAttributeData& OldTestAbilitySet_06);
};
