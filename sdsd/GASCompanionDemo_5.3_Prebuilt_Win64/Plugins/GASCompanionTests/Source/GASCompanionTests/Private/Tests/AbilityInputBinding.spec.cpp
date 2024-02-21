// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.

#include "InputAction.h"
#include "Abilities/GSCAbilitySystemComponent.h"
#include "Components/GSCAbilityInputBindingComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Misc/AutomationTest.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "Utils/GASCompanionTestsUtils.h"

BEGIN_DEFINE_SPEC(FGSCAbilityInputBindingSpec, "GASCompanion.Runtime", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	UWorld* World = nullptr;
	uint64 InitialFrameCounter = 0;
	UInputAction* InputActionFixture = nullptr;

	AGSCModularCharacter* SourceActor = nullptr;
	UAbilitySystemComponent* SourceASC = nullptr;
	APlayerController* SourceController = nullptr;

	AGSCModularCharacter* TargetActor = nullptr;
	UAbilitySystemComponent* TargetASC = nullptr;

END_DEFINE_SPEC(FGSCAbilityInputBindingSpec)

void FGSCAbilityInputBindingSpec::Define()
{
	BeforeEach([this]()
	{
		AddInfo(TEXT("Before Each ..."));

		// Setup tests
		World = FGASCompanionTestsUtils::CreateWorld(InitialFrameCounter);

		// set up the source actor
		SourceActor = World->SpawnActor<AGSCModularCharacter>();
		SourceASC = SourceActor->GetAbilitySystemComponent();

		// Possess and make source actor behave like a player controller actor
		SourceController = World->SpawnActor<APlayerController>();
		SourceController->Possess(SourceActor);

		// set up the destination actor
		TargetActor = World->SpawnActor<AGSCModularCharacter>();
		TargetASC = TargetActor->GetAbilitySystemComponent();
	});

	Describe(TEXT("Ability Input Binding"), [this]()
	{
		BeforeEach([this]()
		{
			// Setup binding component for source actor
			if (UGSCAbilityInputBindingComponent* InputBindingComponent = SourceActor->FindComponentByClass<UGSCAbilityInputBindingComponent>(); !InputBindingComponent)
			{
				InputBindingComponent = NewObject<UGSCAbilityInputBindingComponent>(SourceActor, TEXT("AbilityInputBindingComponent_Test_0"));
				InputBindingComponent->RegisterComponent();
			}

			// Grant ability to Source actor
			if (UGSCAbilitySystemComponent* ASC = Cast<UGSCAbilitySystemComponent>(SourceASC))
			{
				FGSCAbilityInputMapping AbilityInputMapping;
				AbilityInputMapping.Ability = UGameplayAbility::StaticClass();

				InputActionFixture = NewObject<UInputAction>();
				AbilityInputMapping.InputAction = InputActionFixture;
				check(AbilityInputMapping.InputAction);

				ASC->GrantedAbilities.Add(AbilityInputMapping);
				ASC->GrantDefaultAbilitiesAndAttributes(SourceActor, SourceActor);
			}
		});

		It(TEXT("Should return bound input action for a granted ability"), [this]()
		{
			UGSCAbilityInputBindingComponent* SourceInputBindingComponent = SourceActor->FindComponentByClass<UGSCAbilityInputBindingComponent>();
			TestTrue("Source Actor has UGSCAbilityInputBindingComponent", SourceInputBindingComponent != nullptr);

			const TSubclassOf<UGameplayAbility> AbilityToActivate = UGameplayAbility::StaticClass();
			const UGameplayAbility* AbilityCDO = AbilityToActivate.GetDefaultObject();

			bool bSuccess = false;

			UGameplayAbility* ActivatedAbility = nullptr;
			TArray<FGameplayAbilitySpec> ActivatableAbilities = SourceASC->GetActivatableAbilities();
			for (const FGameplayAbilitySpec& Spec : ActivatableAbilities)
			{
				if (Spec.Ability == AbilityCDO)
				{
					bSuccess |= SourceASC->InternalTryActivateAbility(Spec.Handle, FPredictionKey(), &ActivatedAbility);
					break;
				}
			}

			if (!bSuccess)
			{
				AddError(TEXT("Failed to activate ability"));
				return;
			}

			UInputAction* BoundInput = SourceInputBindingComponent->GetBoundInputActionForAbility(ActivatedAbility);
			AddInfo(FString::Printf(TEXT("Bound Input for %s is %s"), *GetNameSafe(ActivatedAbility), *GetNameSafe(BoundInput)));
			TestTrue("AbilityInputBindingComponent::GetBoundInputActionForAbility() should return an InputAction", BoundInput != nullptr);
			TestEqual("Returned InputAction for GetBoundInputActionForAbility() should be the same as the one in granted abilities array", BoundInput, InputActionFixture);
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

		if (SourceController)
		{
			World->EditorDestroyActor(SourceController, false);
		}

		if (TargetActor)
		{
			World->EditorDestroyActor(TargetActor, false);
		}

		// Destroy World
		FGASCompanionTestsUtils::TeardownWorld(World, InitialFrameCounter);
	});
}
