// Copyright 2022 Mickael Daniel. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
struct GASCOMPANIONTESTS_API FGASCompanionTestsNativeTags : public FGameplayTagNativeAdder
{
	FGameplayTag EventLanded;
	FGameplayTag AbilityJump;
	FGameplayTag EventAttributeChangeCalled;
	FGameplayTag EventAbilityEnded;
	FGameplayTag StateTest;
	FGameplayTag StateTest_01;
	FGameplayTag StateTest_02;
	FGameplayTag StateTest_03;
	FGameplayTag StateTest_04;
	FGameplayTag StateTest_05;
	FGameplayTag StateTest_06;
	FGameplayTag StateTest_07;
	FGameplayTag StateTest_08;
	FGameplayTag StateTest_09;

	FORCEINLINE static const FGASCompanionTestsNativeTags& Get() { return NativeTags; }

	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

		EventLanded = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.Event.Landed"));
		AbilityJump = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.Ability.Jump"));
		EventAttributeChangeCalled = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.Event.AttributeChangeCalled"));
		EventAbilityEnded = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.Event.AbilityEnded"));
		StateTest = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.State.Test"));
		StateTest_01 = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.State.Test_01"));
		StateTest_02 = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.State.Test_02"));
		StateTest_03 = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.State.Test_03"));
		StateTest_04 = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.State.Test_04"));
		StateTest_05 = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.State.Test_05"));
		StateTest_06 = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.State.Test_06"));
		StateTest_07 = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.State.Test_07"));
		StateTest_08 = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.State.Test_08"));
		StateTest_09 = Manager.AddNativeGameplayTag(TEXT("GASCompanionTests.State.Test_09"));
	}

private:
	static FGASCompanionTestsNativeTags NativeTags;
};
