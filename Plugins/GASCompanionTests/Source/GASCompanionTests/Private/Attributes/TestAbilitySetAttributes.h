// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "TestAbilitySetAttributes.generated.h"

UCLASS()
class UTestAbilitySetAttributes : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	// Sets default values for this AttributeSet attributes
	UTestAbilitySetAttributes();

    // AttributeSet Overrides
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
        
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_TestAbilitySet_01)
    FGameplayAttributeData TestAbilitySet_01 = 100.0;
    ATTRIBUTE_ACCESSORS(UTestAbilitySetAttributes, TestAbilitySet_01)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_TestAbilitySet_02)
    FGameplayAttributeData TestAbilitySet_02 = 200.0;
    ATTRIBUTE_ACCESSORS(UTestAbilitySetAttributes, TestAbilitySet_02)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_TestAbilitySet_03)
    FGameplayAttributeData TestAbilitySet_03 = 300.0;
    ATTRIBUTE_ACCESSORS(UTestAbilitySetAttributes, TestAbilitySet_03)    

protected:
    
    UFUNCTION()
    virtual void OnRep_TestAbilitySet_01(const FGameplayAttributeData& OldTestAbilitySet_01);

    UFUNCTION()
    virtual void OnRep_TestAbilitySet_02(const FGameplayAttributeData& OldTestAbilitySet_02);

    UFUNCTION()
    virtual void OnRep_TestAbilitySet_03(const FGameplayAttributeData& OldTestAbilitySet_03);
};
