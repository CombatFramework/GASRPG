// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.

#pragma once

#include "GameplayEffect.h"

#if WITH_EDITOR
#include "AssetToolsModule.h"
#endif

#define GSC_TESTS_GET_FIELD_CHECKED(Class, Field) FindFieldChecked<FProperty>(Class::StaticClass(), GET_MEMBER_NAME_CHECKED(Class, Field))
#define GSC_TESTS_CONSTRUCT_CLASS(Class, Name) Class* Name = NewObject<Class>(GetTransientPackage(), FName(TEXT(#Name)))

class UFactory;

class GASCOMPANIONTESTUTILS_API FGASCompanionTestsUtils
{
public:
	/**
	 * Setup an empty world for a test suite.
	 *
	 * @param OutInitialFrameCounter Will be set to the current value of GFrameCounter,
	 * store that to be able to pass it down later to TeardownWorld
	 *
	 * @return Instance of newly created world
	 */
	static UWorld* CreateWorld(uint64& OutInitialFrameCounter);

	/**
	 * Teardown world created earlier for a test suite.
	 *
	 * @param InWorld World instance to destroy and teardown
	 * @param InFrameCounter Frame to restore GFrameCounter to
	 */
	static void TeardownWorld(UWorld* InWorld, uint64 InFrameCounter);

#if WITH_EDITOR
	/**
	 * Create Blueprint helper (not really only blueprint, any asset like UDateAsset)
	 *
	 * eg.
	 *
	 *		FGASCompanionTestsUtils::CreateBlueprint<UDataAssetFactory, UInputAction>(TEXT("IA_Test_Fixture"), TEXT("/Game/Path"));
	 *
	 * Useful to create fixtures at runtime in specs BeforeHooks
	 */
	template <class TAssetFactory, class TAssetClass>
	static TAssetClass* CreateBlueprint(const FString& InAssetName, const FString& InPackagePath)
	{
		const FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

		FString AssetName = InAssetName;
		FString PackageName = InPackagePath;

		const FString DefaultFullPath = PackageName / AssetName;

		TAssetFactory* Factory = NewObject<TAssetFactory>();
		AssetToolsModule.Get().CreateUniqueAssetName(DefaultFullPath, TEXT(""), /*out*/ PackageName, /*out*/ AssetName);
		return Cast<TAssetClass>(AssetToolsModule.Get().CreateAsset(*AssetName, InPackagePath, TAssetClass::StaticClass(), Factory));
	}
#endif

	/** Returns a FAttributeMetaData datatable created dynamically for all GSC Attributes */
	static UDataTable* CreateAttributesDataTable();

	/** Adds a modifier to an instance of a Gameplay Effect */
	template <typename MODIFIER_T>
	static FGameplayModifierInfo& AddModifier(UGameplayEffect* InEffect, FProperty* InProperty, EGameplayModOp::Type InModifierOp, const MODIFIER_T& Magnitude)
	{
		const int32 Idx = InEffect->Modifiers.Num();
		InEffect->Modifiers.SetNum(Idx + 1);
		FGameplayModifierInfo& Info = InEffect->Modifiers[Idx];
		Info.ModifierMagnitude = Magnitude;
		Info.ModifierOp = InModifierOp;
		Info.Attribute.SetUProperty(InProperty);
		return Info;
	}

	/** Copy files utility mainly to ensure fixture files are present in Content folder */
	static bool CopyFolder(const FString& SourceFilepath, const FString& TargetFilepath, FText& ErrorText);


	/** Dynamically create and attach a new actor component to passed in Actor */
	template <class TClass>
	static TClass* AddComponentIfMissing(AActor* InActor, const FName& InComponentName = NAME_None)
	{
		check(InActor);

		TClass* Component = InActor->FindComponentByClass<TClass>();
		if (Component)
		{
			return Component;
		}

		FName ComponentName = InComponentName;
		if (ComponentName == NAME_None)
		{
			ComponentName = FName(*FString::Printf(TEXT("%s_%s"), *GetNameSafe(TClass::StaticClass()), *FGuid::NewGuid().ToString()));
		}

		Component = NewObject<TClass>(InActor, ComponentName);
		if (Component)
		{
			Component->RegisterComponent();
		}

		return Component;
	}
};
