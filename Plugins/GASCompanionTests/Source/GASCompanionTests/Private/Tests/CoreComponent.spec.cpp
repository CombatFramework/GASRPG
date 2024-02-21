// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.

#include "CoreMinimal.h"
#include "Editor.h"
#include "GASCompanionTestsNativeTags.h"
#include "Abilities/Attributes/GSCAttributeSet.h"
#include "Misc/AutomationTest.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "Utils/GASCompanionTestsUtils.h"

BEGIN_DEFINE_SPEC(FGSCCoreComponentSpec, "GASCompanion.Runtime", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UWorld* World = nullptr;

	AGSCModularCharacter* SourceActor = nullptr;
	UAbilitySystemComponent* SourceASC = nullptr;

	static constexpr const TCHAR* FixtureFile = TEXT("/GASCompanionTests/Fixtures/BP_CoreComponentSpec_MockupCharacter.BP_CoreComponentSpec_MockupCharacter_C");

	AGSCModularCharacter* TargetActor = nullptr;
	UAbilitySystemComponent* TargetASC = nullptr;

	uint64 InitialFrameCounter = 0;
END_DEFINE_SPEC(FGSCCoreComponentSpec)

void FGSCCoreComponentSpec::Define()
{
	Describe(TEXT("Core Component"), [this]()
	{
		BeforeEach([this]()
		{
			if (!GEditor)
			{
				AddError(TEXT("This test must be run in editor mode (no -game)"));
				return;	
			}
			
			// Setup tests
			World = FGASCompanionTestsUtils::CreateWorld(InitialFrameCounter);

			// set up the source actor
			SourceActor = World->SpawnActor<AGSCModularCharacter>();
			SourceASC = SourceActor->GetAbilitySystemComponent();

			UClass* ActorClass = StaticLoadClass(UObject::StaticClass(), nullptr, FixtureFile);
			if (!IsValid(ActorClass))
			{
				AddError(FString::Printf(TEXT("Unable to load %s"), FixtureFile));
				AddError(TEXT("Make sure to setup `Plugins > GASCompanion Tests > Setup Tests` to true before running tests."));
				return;
			}

			// set up the destination actor
			TargetActor = Cast<AGSCModularCharacter>(World->SpawnActor(ActorClass, nullptr, nullptr, FActorSpawnParameters()));
			if (!TargetActor)
			{
				AddError(FString::Printf(TEXT("Unable to setup target actor from %s"), *GetNameSafe(ActorClass)));
				return;
			}

			TargetASC = TargetActor->GetAbilitySystemComponent();
			if (!TargetASC)
			{
				AddError(FString::Printf(TEXT("Unable to setup target ASC from %s"), *GetNameSafe(ActorClass)));
				return;
			}

			// Emulate multiple possession / owner rep to ensure delegates are only registered once
			TargetASC->InitAbilityActorInfo(TargetActor, TargetActor);
			TargetASC->InitAbilityActorInfo(TargetActor, TargetActor);
			TargetASC->InitAbilityActorInfo(TargetActor, TargetActor);
		});

		LatentIt(TEXT("should trigger OnAttributeChange once"), [this](const FDoneDelegate& Done)
		{

			int32 TimesCalled = 0;

			// Blueprint fixture will send this event anytime an attribute changes
			const FGameplayTag EventTag = FGASCompanionTestsNativeTags::Get().EventAttributeChangeCalled;
			TargetASC->GenericGameplayEventCallbacks.FindOrAdd(EventTag).AddLambda([this, &TimesCalled, EventTag](const FGameplayEventData* Payload)
			{
				++TimesCalled;
				AddInfo(FString::Printf(TEXT("Received gameplay event ... %s - TimesCalled: %d"), *EventTag.ToString(), TimesCalled));
			});
			
			// just try and reduce the health attribute
			{
				constexpr float DamageValue = 10.f;
				GSC_TESTS_CONSTRUCT_CLASS(UGameplayEffect, BaseDmgEffect);
				FGASCompanionTestsUtils::AddModifier(
					BaseDmgEffect,
					GSC_TESTS_GET_FIELD_CHECKED(UGSCAttributeSet, Health),
					EGameplayModOp::Additive,
					FScalableFloat(-DamageValue)
				);
				BaseDmgEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

				SourceASC->ApplyGameplayEffectToTarget(BaseDmgEffect, TargetASC);
			}

			AddInfo(FString::Printf(TEXT("New Health: %f"), TargetASC->GetNumericAttributeBase(UGSCAttributeSet::GetHealthAttribute())));

			GEditor->GetTimerManager()->SetTimerForNextTick([this, TimesCalled, Done]()
			{
				FGameplayTagContainer Tags;
				TargetASC->GetOwnedGameplayTags(Tags);

				// int32 TimesCalled = TargetStorage->GetValueAsInt(TEXT("TimesCalled"));

				AddInfo(FString::Printf(TEXT("Tags: %s"), *Tags.ToString()));
				TestEqual(TEXT("TimesCalled"), TimesCalled, 1);
				Done.Execute();
			});
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

		if (TargetASC)
		{
			const FGameplayTag EventTag = FGASCompanionTestsNativeTags::Get().EventAttributeChangeCalled;
			TargetASC->GenericGameplayEventCallbacks.FindOrAdd(EventTag).RemoveAll(this);
		}


		if (TargetActor)
		{
			World->EditorDestroyActor(TargetActor, false);
		}

		// Destroy World
		FGASCompanionTestsUtils::TeardownWorld(World, InitialFrameCounter);
	});
}
