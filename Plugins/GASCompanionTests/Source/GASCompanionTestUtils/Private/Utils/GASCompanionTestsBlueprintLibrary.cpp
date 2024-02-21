// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.


#include "Utils/GASCompanionTestsBlueprintLibrary.h"

#include "EnhancedPlayerInput.h"
#include "FunctionalTest.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"

#if WITH_EDITOR
#include "Settings/LevelEditorPlaySettings.h"
#endif

void UGASCompanionTestsBlueprintLibrary::ApplyEnhancedInputAction(UObject* Context, const UInputAction* Action)
{
	ApplyEnhancedInputActionWithValue(Context, Action, FInputActionValue(0.5f));
}

void UGASCompanionTestsBlueprintLibrary::ApplyEnhancedInputActionWithValue(UObject* Context, const UInputAction* Action, const FInputActionValue& InRawValue)
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(Context, 0);

	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Error, TEXT("UGASCompanionTestsBlueprintLibrary::ApplyEnhancedInputAction Invalid Player PC"));
		return;
	}

	const TObjectPtr<UPlayerInput> PlayerInput = PlayerController->PlayerInput;
	if (!PlayerInput)
	{
		UE_LOG(LogTemp, Error, TEXT("UGASCompanionTestsBlueprintLibrary::ApplyEnhancedInputAction Invalid Player Input"));
		return;
	}

	UEnhancedPlayerInput* EnhancedPlayerInput = Cast<UEnhancedPlayerInput>(PlayerInput);
	if (!EnhancedPlayerInput)
	{
		UE_LOG(LogTemp, Error, TEXT("UGASCompanionTestsBlueprintLibrary::ApplyEnhancedInputAction Invalid Enhanced Player Input"));
		return;
	}

	EnhancedPlayerInput->InjectInputForAction(Action, InRawValue);
}

void UGASCompanionTestsBlueprintLibrary::AddInfo(UObject* Context, const FString& Message)
{
	AFunctionalTest* Test = Cast<AFunctionalTest>(Context);
	if (!Test)
	{
		UE_LOG(LogTemp, Error, TEXT("UGASCompanionTestsBlueprintLibrary::AddInfo Context (%s) is not a functional test. Ensure you're calling this method from within an AFunctionalTest event graph"), *GetNameSafe(Context));
		return;
	}

	Test->LogStep(ELogVerbosity::Display, *Message);
}

void UGASCompanionTestsBlueprintLibrary::AddExpectedLogErrorForClientPie(const UObject* WorldContextObject, FString ExpectedPatternString, int32 Occurrences, bool ExactMatch)
{
#if WITH_EDITOR
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	
	// Can't rely on World NetMode, it's always set to DedicatedServer when in PIE (and tests are executed in Editor via Session Frontend)
	const ULevelEditorPlaySettings* LevelEditorPlaySettings = GetDefault<ULevelEditorPlaySettings>();
	EPlayNetMode PlayNetMode = PIE_Standalone;
	LevelEditorPlaySettings->GetPlayNetMode(PlayNetMode);

	if (PlayNetMode == PIE_Client)
	{
		if (FAutomationTestBase* CurrentTest = FAutomationTestFramework::Get().GetCurrentTest())
		{
			AddInfo(const_cast<UObject*>(WorldContextObject), FString::Printf(TEXT("Adding Expected Pattern String \"%s\" for Client Net Mode (World: %s)"), *ExpectedPatternString, *GetNameSafe(World)));
			CurrentTest->AddExpectedError(ExpectedPatternString, ExactMatch ? EAutomationExpectedErrorFlags::Exact : EAutomationExpectedErrorFlags::Contains, Occurrences);
		}
	}
#endif
}
