// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GASCompanionTestsBlueprintLibrary.generated.h"

class UInputAction;

/**
 * Provides test helper such as input simulation for usage in functional tests
 */
UCLASS()
class GASCOMPANIONTESTUTILS_API UGASCompanionTestsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Applies the enhanced input action once */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|Tests", meta = (HidePin = "Context", DefaultToSelf = "Context"))
	static void ApplyEnhancedInputAction(UObject* Context, const UInputAction* Action);
	
	/** Applies the enhanced input action once with provided input action value */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|Tests", meta = (HidePin = "Context", DefaultToSelf = "Context"))
	static void ApplyEnhancedInputActionWithValue(UObject* Context, const UInputAction* Action, const FInputActionValue& InRawValue);

	/** Functional testing helper to add a log with Display level via the automation framework. Only valid to call from within a Functional Test event graph */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|Tests", meta = (HidePin = "Context", DefaultToSelf = "Context"))
	static void AddInfo(UObject* Context, const FString& Message);
	
	/**
	 * Mute the report of log error and warning matching a pattern during an automated test.
	 *
	 * This is very specific to how we run functional tests and how we want to be able to test with
	 * NetMode set to Client (when testing from within Editor's Session Frontend and NetMode client or standalone,
	 * as well as when run with Gauntlet and Client Role)
	 *
	 * In case of Gauntlet, the expected error logs are added by the Gauntlet Controller, not from within the Functional Test Blueprint.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|Tests", meta=(WorldContext="WorldContextObject", AdvancedDisplay = "Occurrences, ExactMatch"))
	static void AddExpectedLogErrorForClientPie(const UObject* WorldContextObject, FString ExpectedPatternString, int32 Occurrences = 1, bool ExactMatch = false);
};
