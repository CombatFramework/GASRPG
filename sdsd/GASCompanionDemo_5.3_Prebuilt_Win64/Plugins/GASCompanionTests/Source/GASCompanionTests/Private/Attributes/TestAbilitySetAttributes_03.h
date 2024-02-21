// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "TestAbilitySetAttributes_03.generated.h"

UCLASS()
class UTestAbilitySetAttributes_03 : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	// Sets default values for this AttributeSet attributes
	UTestAbilitySetAttributes_03();

    // AttributeSet Overrides
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
        
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_TestAbilitySet_07)
    FGameplayAttributeData TestAbilitySet_07 = 400.f;
    ATTRIBUTE_ACCESSORS(UTestAbilitySetAttributes_03, TestAbilitySet_07)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_TestAbilitySet_08)
    FGameplayAttributeData TestAbilitySet_08 = 500.f;
    ATTRIBUTE_ACCESSORS(UTestAbilitySetAttributes_03, TestAbilitySet_08)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_TestAbilitySet_09)
    FGameplayAttributeData TestAbilitySet_09 = 600.f;
    ATTRIBUTE_ACCESSORS(UTestAbilitySetAttributes_03, TestAbilitySet_09)    

protected:
    
    UFUNCTION()
    virtual void OnRep_TestAbilitySet_07(const FGameplayAttributeData& OldTestAbilitySet_07);

    UFUNCTION()
    virtual void OnRep_TestAbilitySet_08(const FGameplayAttributeData& OldTestAbilitySet_08);

    UFUNCTION()
    virtual void OnRep_TestAbilitySet_09(const FGameplayAttributeData& OldTestAbilitySet_09);
};
