#include "pch.h"
#include "FortniteGame/Public/STW/FortGameModeOutpost.h"

void FortGameModeOutpost::CreateAIDirector(AFortGameModeOutpost* FortGameModeOutpost)
{
	UClass* AIDirectorClass = AFortAIDirector::StaticClass();

	if (UFortGameData::Get()->AIDirectors.IsValidIndex(FortGameModeOutpost->OverrideAIDirectorIndex))
		AIDirectorClass = UFortGameData::Get()->AIDirectors[FortGameModeOutpost->OverrideAIDirectorIndex].Get();

	FortGameModeOutpost->AIDirector = GWorld->SpawnActor<AFortAIDirector>(FVector(), FRotator(), AIDirectorClass, FortGameModeOutpost);
	FortGameModeOutpost->AIDirector->Activate();
}

void FortGameModeOutpost::InitGameState(AFortGameModeOutpost* FortGameModeOutpost)
{
	Originals::InitGameState(FortGameModeOutpost);

	if (AFortGameStateOutpost* FortGameStateOutpost = Cast<AFortGameStateOutpost>(FortGameModeOutpost->GameState))
	{
		UFortGameData* GameData = UFortGameData::Get();

		if (FortGameStateOutpost->MissionManager == NULL)
			FortGameStateOutpost->MissionManager = GWorld->SpawnActor<AFortMissionManager>(FVector(), FRotator(), FortGameModeOutpost->MissionManagerClass.Get(), FortGameStateOutpost);

		FortGameStateOutpost->MissionManager->BluGloManager = GWorld->SpawnActor<AFortBluGloManager>(FVector(), FRotator(), GameData->BluGloManagerClass.Get(), FortGameStateOutpost->MissionManager);
		FortGameStateOutpost->OnRep_MissionManager();
	}

	FortGameModeOutpost->MissionGenerationManager = GWorld->SpawnActor<AFortMissionGenerationManager>(FVector(), FRotator(), AFortMissionGenerationManager::StaticClass(), FortGameModeOutpost);
}

void FortGameModeOutpost::FinishWorldInitialization(AFortGameModeOutpost* FortGameModeOutpost, AFortWorldManager* WorldManager)
{
	Originals::FinishWorldInitialization(FortGameModeOutpost, WorldManager);

	if (WorldManager != NULL)
		WorldManager->FinishWorldInitialization();

	FortGameModeOutpost->FinishWorldInitialization(WorldManager);

	SetConsoleTitleA("Phantom | Ready");
}

APawn* FortGameModeOutpost::SpawnDefaultPawnFor_Implementation(AFortGameModeOutpost* FortGameModeOutpost, AController* NewPlayer, AActor* StartSpot)
{
	if (AFortPlayerControllerZone* FortPlayerController = Cast<AFortPlayerControllerZone>(NewPlayer))
	{
		if (FortPlayerController->QuickBars == NULL)
			FortPlayerController->QuickBars = GWorld->SpawnActor<AFortQuickBars>(FVector(), FRotator(), AFortQuickBars::StaticClass(), FortPlayerController);

		if (AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory())
		{
			// stw doesnt want startingitems so im forced to do this!!!!

			UFortGameData* GameData = UFortGameData::Get();

			for (const FItemDefinitionAndCount& InventoryItem : GameData->FastLoadDefaultInventoryList)
			{
				WorldInventory->AddItem(InventoryItem.ItemDefinition.LoadSynchronous(), InventoryItem.Count);
			}
		}
	}

	return FortGameModeOutpost->SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}

void FortGameModeOutpost::Setup()
{
	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0xC08 / 8, CreateAIDirector);
	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0x610 / 8, SpawnDefaultPawnFor_Implementation);

	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0xA40 / 8, FinishWorldInitialization, (void**)&Originals::FinishWorldInitialization);
	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0x660 / 8, InitGameState, (void**)&Originals::InitGameState);
}