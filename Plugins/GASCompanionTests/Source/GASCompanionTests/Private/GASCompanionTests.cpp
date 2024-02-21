// Copyright 2021-2022 Mickael Daniel. All Rights Reserved.

#include "GASCompanionTests.h"

#include "IAutomationControllerModule.h"
#include "Utils/GASCompanionTestsUtils.h"

#define LOCTEXT_NAMESPACE "FGASCompanionTestsModule"

void FGASCompanionTestsModule::StartupModule()
{
}

void FGASCompanionTestsModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGASCompanionTestsModule, GASCompanionTests)
