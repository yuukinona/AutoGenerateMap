// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MapGener.h"
#include "MapGenerEdMode.h"

#define LOCTEXT_NAMESPACE "FMapGenerModule"

void FMapGenerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FMapGenerEdMode>(FMapGenerEdMode::EM_MapGenerEdModeId, LOCTEXT("MapGenerEdModeName", "MapGenerEdMode"), FSlateIcon(), true);
}

void FMapGenerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FMapGenerEdMode::EM_MapGenerEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMapGenerModule, MapGener)