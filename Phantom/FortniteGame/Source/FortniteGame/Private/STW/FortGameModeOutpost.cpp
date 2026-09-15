#include "pch.h"
#include "FortniteGame/Public/STW/FortGameModeOutpost.h"

float GetDefaultHalfHeight(AFortPlayerPawn* FortPlayerPawn)
{
	return 800.f;
}

void FortGameModeOutpost::InitGameState(AFortGameModeOutpost* FortGameModeOutpost)
{
	Originals::InitGameState(FortGameModeOutpost);

	UFortGameData* GameData = UFortGameData::Get();

	if (AFortGameStateOutpost* FortGameStateOutpost = Cast<AFortGameStateOutpost>(FortGameModeOutpost->GameState))
	{
		if (FortGameStateOutpost->MissionManager == NULL)
			FortGameStateOutpost->MissionManager = GWorld->SpawnActor<AFortMissionManager>(FVector(), FRotator(), FortGameModeOutpost->MissionManagerClass.Get(), FortGameStateOutpost);

		FortGameStateOutpost->MissionManager->BluGloManager = GWorld->SpawnActor<AFortBluGloManager>(FVector(), FRotator(), GameData->BluGloManagerClass.Get(), FortGameStateOutpost->MissionManager);
		FortGameStateOutpost->OnRep_MissionManager();
	}

	FortGameModeOutpost->MissionGenerationManager = GWorld->SpawnActor<AFortMissionGenerationManager>(FVector(), FRotator(), AFortMissionGenerationManager::StaticClass(), FortGameModeOutpost);

	if (FortGameModeOutpost->SharedMissionLists == NULL)
		FortGameModeOutpost->SharedMissionLists = NewObject<UFortSharedMissionLists>(FortGameModeOutpost, GameData->SharedMissionListsClass.Get());

	Utils::Virtual(AFortPlayerPawn::GetDefaultObj(), 0x640 / 8, GetDefaultHalfHeight);
}

void FortGameModeOutpost::HandleStartingNewPlayer(AFortGameModeOutpost* FortGameModeOutpost, APlayerController* NewPlayer)
{
	Originals::HandleStartingNewPlayer(FortGameModeOutpost, NewPlayer);

	if (AFortPlayerStateOutpost* FortPlayerStateOutpost = Cast<AFortPlayerStateOutpost>(NewPlayer->PlayerState))
	{
		FortPlayerStateOutpost->SetIsWorldDataOwner(true);
	}
}

void FortGameModeOutpost::Setup()
{
	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0x660 / 8, InitGameState, (void**)&Originals::InitGameState);
	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0x640 / 8, HandleStartingNewPlayer, (void**)&Originals::HandleStartingNewPlayer);
}