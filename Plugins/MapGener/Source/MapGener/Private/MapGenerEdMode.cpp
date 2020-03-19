// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MapGenerEdMode.h"
#include "MapGenerEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FMapGenerEdMode::EM_MapGenerEdModeId = TEXT("EM_MapGenerEdMode");

FMapGenerEdMode::FMapGenerEdMode()
{

}

FMapGenerEdMode::~FMapGenerEdMode()
{

}

void FMapGenerEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FMapGenerEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FMapGenerEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FMapGenerEdMode::UsesToolkits() const
{
	return true;
}




