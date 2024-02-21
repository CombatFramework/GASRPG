// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.

#include "InputAction.h"
#include "Abilities/GSCAbilitySystemComponent.h"
#include "Abilities/Attributes/GSCAttributeSet.h"
#include "Components/GSCAbilityInputBindingComponent.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "Utils/GASCompanionTestsUtils.h"


BEGIN_DEFINE_SPEC(FGSCModularCharactersSpec, "GASCompanion.Runtime", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UWorld* World = nullptr;
	uint64 InitialFrameCounter = 0;
	UInputAction* InputActionFixture = nullptr;

	AGSCModularCharacter* SourceActor = nullptr;
	UAbilitySystemComponent* SourceASC = nullptr;

	AGSCModularCharacter* TargetActor = nullptr;
	UAbilitySystemComponent* TargetASC = nullptr;
END_DEFINE_SPEC(FGSCModularCharactersSpec)

void FGSCModularCharactersSpec::Define()
{
	BeforeEach([this]()
	{
		AddInfo(TEXT("Before Each ..."));

		// Setup tests
		World = FGASCompanionTestsUtils::CreateWorld(InitialFrameCounter);

		// set up the source actor
		SourceActor = World->SpawnActor<AGSCModularCharacter>();
		SourceASC = SourceActor->GetAbilitySystemComponent();

		// set up the destination actor
		TargetActor = World->SpawnActor<AGSCModularCharacter>();
		TargetASC = TargetActor->GetAbilitySystemComponent();
	});

	Describe(TEXT("Modular Characters"), [this]()
	{
		BeforeEach([this]()
		{
			// Grant attributes to Source
			if (UGSCAbilitySystemComponent* ASCSource = Cast<UGSCAbilitySystemComponent>(SourceASC))
			{
				FGSCAttributeSetDefinition AttributesDefinition;
				AttributesDefinition.AttributeSet = UGSCAttributeSet::StaticClass();
				AttributesDefinition.InitializationData = FGASCompanionTestsUtils::CreateAttributesDataTable();
				ASCSource->GrantedAttributes.Add(AttributesDefinition);

				ASCSource->GrantDefaultAbilitiesAndAttributes(SourceActor, SourceActor);
			}

			// Grant attributes to Target
			if (UGSCAbilitySystemComponent* ASCTarget = Cast<UGSCAbilitySystemComponent>(TargetASC))
			{
				FGSCAttributeSetDefinition AttributesDefinition;
				AttributesDefinition.AttributeSet = UGSCAttributeSet::StaticClass();
				AttributesDefinition.InitializationData = FGASCompanionTestsUtils::CreateAttributesDataTable();
				ASCTarget->GrantedAttributes.Add(AttributesDefinition);

				ASCTarget->GrantDefaultAbilitiesAndAttributes(TargetActor, TargetActor);
			}
		});

		It(TEXT("Should spawned actor be initialized with an AbilitySystemComponent"), [this]()
		{
			UGSCAbilitySystemComponent* SourceModularASC = SourceActor->FindComponentByClass<UGSCAbilitySystemComponent>();
			TestTrue("Source Actor has UGSCAbilitySystemComponent", SourceModularASC != nullptr);
			TestTrue("Source Actor ASC is the same returned by IAbilitySystemInterface", SourceModularASC == SourceASC);

			UGSCAbilitySystemComponent* TargetModularASC = TargetActor->FindComponentByClass<UGSCAbilitySystemComponent>();
			TestTrue("Target Actor has UGSCAbilitySystemComponent", TargetModularASC != nullptr);
			TestTrue("Source Actor ASC is the same returned by IAbilitySystemInterface", TargetModularASC == TargetASC);

			TestNotEqual("Source and Target ASCs are not the same address", SourceModularASC, TargetModularASC);
		});

		It(TEXT("Should have AttributeSet granted and initialized"), [this]()
		{
			TestTrue("Source ASC has UGSCAttributeSet granted", SourceASC->HasAttributeSetForAttribute(UGSCAttributeSet::GetHealthAttribute()));
			TestTrue("Target ASC has UGSCAttributeSet granted", TargetASC->HasAttributeSetForAttribute(UGSCAttributeSet::GetHealthAttribute()));

			TestEqual("Source ASC AttributeSet Health is initialized to 500.f", SourceASC->GetNumericAttributeBase(UGSCAttributeSet::GetHealthAttribute()), 500.f);
			TestEqual("Target ASC AttributeSet Health is initialized to 500.f", TargetASC->GetNumericAttributeBase(UGSCAttributeSet::GetHealthAttribute()), 500.f);
		});

		It(TEXT("Should health be reduced via GameplayEffect"), [this]()
		{
			constexpr float DamageValue = 5.f;
			const float StartingHealth = TargetASC->GetNumericAttributeBase(UGSCAttributeSet::GetHealthAttribute());

			// just try and reduce the health attribute
			{
				GSC_TESTS_CONSTRUCT_CLASS(UGameplayEffect, BaseDmgEffect);
				FGASCompanionTestsUtils::AddModifier(
					BaseDmgEffect,
					GSC_TESTS_GET_FIELD_CHECKED(UGSCAttributeSet, Health),
					EGameplayModOp::Additive,
					FScalableFloat(-DamageValue)
				);
				BaseDmgEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

				SourceASC->ApplyGameplayEffectToTarget(BaseDmgEffect, TargetASC, 1.f);
			}

			// make sure health was reduced
			TestEqual(TEXT("Health Reduced"), TargetASC->GetNumericAttributeBase(UGSCAttributeSet::GetHealthAttribute()), StartingHealth - DamageValue);
		});

		It(TEXT("Should health be reduced using Damage Meta Attribute"), [this]()
		{
			constexpr float DamageValue = 5.f;
			const float StartingHealth = TargetASC->GetNumericAttributeBase(UGSCAttributeSet::GetHealthAttribute());

			// just try and reduce the health attribute, using damage meta attribute
			{
				GSC_TESTS_CONSTRUCT_CLASS(UGameplayEffect, BaseDmgEffect);
				FGASCompanionTestsUtils::AddModifier(
					BaseDmgEffect,
					GSC_TESTS_GET_FIELD_CHECKED(UGSCAttributeSet, Damage),
					EGameplayModOp::Additive,
					FScalableFloat(DamageValue)
				);
				BaseDmgEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

				SourceASC->ApplyGameplayEffectToTarget(BaseDmgEffect, TargetASC, 1.f);
			}

			// Now we should have lost some health
			TestEqual(TEXT("Health Reduced via Damage Attribute (Additive Operation)"), TargetASC->GetNumericAttributeBase(UGSCAttributeSet::GetHealthAttribute()), StartingHealth - DamageValue);

			// Confirm the damage attribute itself was reset to 0 when it was applied to health
			TestEqual(TEXT("Damage Applied"), TargetASC->GetSet<UGSCAttributeSet>()->GetDamage(), 0.f);

			// just try and reduce the health attribute, using damage meta attribute (now using override operation)
			{
				GSC_TESTS_CONSTRUCT_CLASS(UGameplayEffect, BaseDmgEffect);
				FGASCompanionTestsUtils::AddModifier(
					BaseDmgEffect,
					GSC_TESTS_GET_FIELD_CHECKED(UGSCAttributeSet, Damage),
					EGameplayModOp::Override,
					FScalableFloat(DamageValue)
				);
				BaseDmgEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

				SourceASC->ApplyGameplayEffectToTarget(BaseDmgEffect, TargetASC, 1.f);
			}

			// make sure health was reduced
			TestEqual(TEXT("Health Reduced via Damage Attribute (Override Operation)"), TargetASC->GetNumericAttributeBase(UGSCAttributeSet::GetHealthAttribute()), StartingHealth - (DamageValue * 2));

			// Confirm the damage attribute itself was reset to 0 when it was applied to health
			TestEqual(TEXT("Damage Applied"), TargetASC->GetNumericAttributeBase(UGSCAttributeSet::GetDamageAttribute()), 0.f);
		});
	});

	AfterEach([this]()
	{
		AddInfo(TEXT("After Each ..."));

		// Destroy the actors
		if (SourceActor)
		{
			World->EditorDestroyActor(SourceActor, false);
		}

		if (TargetActor)
		{
			World->EditorDestroyActor(TargetActor, false);
		}

		// Destroy World
		FGASCompanionTestsUtils::TeardownWorld(World, InitialFrameCounter);
	});
}
