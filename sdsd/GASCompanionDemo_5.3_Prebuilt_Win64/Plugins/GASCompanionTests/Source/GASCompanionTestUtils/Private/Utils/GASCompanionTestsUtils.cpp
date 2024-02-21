// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.


#include "Utils/GASCompanionTestsUtils.h"

#include "UnrealEngine.h"
#include "Engine/World.h"
#include "HAL/FileManager.h"

UWorld* FGASCompanionTestsUtils::CreateWorld(uint64& OutInitialFrameCounter)
{
	// Setup tests
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	check(World);
	
	FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
	WorldContext.SetCurrentWorld(World);

	const FURL URL;
	World->InitializeActorsForPlay(URL);
	World->BeginPlay();

	OutInitialFrameCounter = GFrameCounter;

	return World;
}

void FGASCompanionTestsUtils::TeardownWorld(UWorld* InWorld, uint64 InFrameCounter)
{
	GFrameCounter = InFrameCounter;

	// Teardown tests
	GEngine->DestroyWorldContext(InWorld);
	InWorld->DestroyWorld(false);
}

UDataTable* FGASCompanionTestsUtils::CreateAttributesDataTable()
{
	FString CSV(TEXT("---,BaseValue,MinValue,MaxValue,DerivedAttributeInfo,bCanStack"));
	CSV.Append(TEXT("\r\nGSCAttributeSet.MaxHealth,\"500.000000\",\"0.000000\",\"1.000000\",\"\",\"False\""));
	CSV.Append(TEXT("\r\nGSCAttributeSet.Health,\"500.000000\",\"0.000000\",\"1.000000\",\"\",\"False\""));
	CSV.Append(TEXT("\r\nGSCAttributeSet.MaxStamina,\"90.000000\",\"0.000000\",\"1.000000\",\"\",\"False\""));
	CSV.Append(TEXT("\r\nGSCAttributeSet.Stamina,\"90.000000\",\"0.000000\",\"1.000000\",\"\",\"False\""));
	CSV.Append(TEXT("\r\nGSCAttributeSet.MaxMana,\"100.000000\",\"0.000000\",\"1.000000\",\"\",\"False\""));
	CSV.Append(TEXT("\r\nGSCAttributeSet.Mana,\"100.000000\",\"0.000000\",\"1.000000\",\"\",\"False\""));
	CSV.Append(TEXT("\r\nGSCAttributeSet.StaminaRegenRate,\"45.000000\",\"0.000000\",\"1.000000\",\"\",\"False\""));

	UDataTable* DataTable = NewObject<UDataTable>(GetTransientPackage(), FName(TEXT("TempDataTable")));
	DataTable->RowStruct = FAttributeMetaData::StaticStruct();
	DataTable->CreateTableFromCSVString(CSV);

	if (const FAttributeMetaData* Row = reinterpret_cast<const FAttributeMetaData*>(DataTable->GetRowMap()["GSCAttributeSet.MaxHealth"]))
	{
		check(Row->BaseValue == 500.f);
	}
	return DataTable;
}

bool FGASCompanionTestsUtils::CopyFolder(const FString& SourceFilepath, const FString& TargetFilepath, FText& ErrorText)
{
	if (IFileManager::Get().DirectoryExists(*TargetFilepath))
	{
		// Stop here, directory already there
		ErrorText = NSLOCTEXT("GASCompanionTestsUtils", "CopyFolder_DirectoryExists", "Stop here, directory already there.");
		return false;
	}

	// Copy all files from SourceFilepath to TargetFilepath
	TArray<FString> Files;
	IFileManager::Get().FindFilesRecursive(Files, *SourceFilepath, TEXT("*"), true, false);
	for (const FString& File : Files)
	{
		FString RelativeFilepath = File;
		RelativeFilepath.RemoveFromStart(SourceFilepath);

		const FString TargetDestination = TargetFilepath / RelativeFilepath;
		UE_LOG(LogTemp, Display, TEXT("\tFGASCompanionTestsUtils::CopyFolder Try copy %s to %s"), *File, *TargetDestination);
		if (IFileManager::Get().Copy(*TargetDestination, *File, true) != COPY_OK)
		{
			ErrorText = FText::Format(
				NSLOCTEXT("GASCompanionTestsUtils", "FailedCopy", "Failed to copy {0} to {1}"),
				FText::FromString(File),
				FText::FromString(TargetDestination)
			);
			return false;
		}
	}

	return true;
}
