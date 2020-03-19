// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MapGenerEdModeToolkit.h"
#include "MapGenerEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#include "FileHelpers.h"

#define LOCTEXT_NAMESPACE "FMapGenerEdModeToolkit"

FMapGenerEdModeToolkit::FMapGenerEdModeToolkit()
{
}

void FMapGenerEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}
	};

	struct GeneratorButton
	{
		static FReply OnButtonClick(FVector InOffset)
		{
			/*
			ULevel* level = GEditor->EditorWorld->GetCurrentLevel();

			for (AActor* Actor : level->Actors)
			{
				// Actor action 
				if (Actor->GetClass()->GetName() == FString(""))
				{

				}			
			}*/

			//GEditor->GameViewport->GetWorld()->actors
			//USelection* SelectedActors = GEditor->GetSelectedActors();
			
			UWorld* editorWorld = GEditor->GetEditorWorldContext().World();
			if (editorWorld)
			{
				FString longPackageName = "";
				FString longFilename = "";
				if (FPackageName::SearchForPackageOnDisk("Procedural_base", &longPackageName, &longFilename))
				{
					FEditorFileUtils::SaveDirtyPackages(false, true, false, false);
					FEditorFileUtils::LoadMap(longPackageName, false, true);
				}
				//GEditor->SelectAllActorsWithClass(false);

				USelection* SelectedActors = GEditor->GetSelectedActors();
				for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
				{
					if (AActor* LevelActor = Cast<AActor>(*Iter))
					{
						UE_LOG(LogTemp, Warning, TEXT("%s"), *LevelActor->GetClass()->GetName());
					}
				}

// 				TArray<UPackage*> arr;
// 				FEditorFileUtils::GetDirtyWorldPackages(arr);
// 				for (UPackage* package : arr)
// 				{
// 					FString fullName = package->GetFullName();
// 					UE_LOG(LogTemp, Warning,TEXT("%s"), *fullName);
// 				}
// 
// 				FEditorFileUtils::LoadMap(TEXT("/Game/Content/ProceduralLevel/Procedural_base.umap"), false, true);
			}
			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&GeneratorButton::OnButtonClick, InOffset);
		}
	};

	const float Factor = 256.0f;

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Select some actors and move them around using buttons below"))
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.AutoHeight()
				[
					GeneratorButton::MakeButton(LOCTEXT("OpenLevel", "Up"), FVector(0, 0, Factor))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("UpButtonLabel", "Up"), FVector(0, 0, Factor))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						Locals::MakeButton(LOCTEXT("LeftButtonLabel", "Left"), FVector(0, -Factor, 0))
					]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("RightButtonLabel", "Right"), FVector(0, Factor, 0))
						]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("DownButtonLabel", "Down"), FVector(0, 0, -Factor))
				]

		];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FMapGenerEdModeToolkit::GetToolkitFName() const
{
	return FName("MapGenerEdMode");
}

FText FMapGenerEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("MapGenerEdModeToolkit", "DisplayName", "MapGenerEdMode Tool");
}

class FEdMode* FMapGenerEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FMapGenerEdMode::EM_MapGenerEdModeId);
}

#undef LOCTEXT_NAMESPACE
