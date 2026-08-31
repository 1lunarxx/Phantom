#include "pch.h"
#include "FortniteGame/Public/Athena/FortGameModeAthena.h"

void FortGameModeAthena::FinishWorldInitialization(AFortGameModeAthena* FortGameModeAthena, AFortWorldManager* WorldManager)
{
	if (!FortGameModeAthena->bWorldIsReady)
	{
		FortGameModeAthena->bWorldIsReady = true;
		SetConsoleTitleA("Phantom | Ready");
	}

	Originals::FinishWorldInitialization(FortGameModeAthena, WorldManager);
}

void FortGameModeAthena::InitGameState(AFortGameModeAthena* FortGameModeAthena)
{
	Originals::InitGameState(FortGameModeAthena);

	if (UFortPlaylistAthena* Playlist = FortGameModeAthena->PlaylistManager->GetAthenaPlaylist(FName(L"Playlist_DefaultSolo")))
	{
		FortGameModeAthena->CurrentPlaylistId = Playlist->PlaylistId;
		FortGameModeAthena->CurrentPlaylistName = Playlist->PlaylistName;

		FortGameModeAthena->GameSession->MaxPlayers = Playlist->MaxPlayers;

		if (AFortGameStateAthena* FortGameStateAthena = Cast<AFortGameStateAthena>(FortGameModeAthena->GameState))
		{
			FortGameStateAthena->CurrentPlaylistId = Playlist->PlaylistId;

			FortGameStateAthena->CurrentPlaylistInfo.BasePlaylist = Playlist;
			FortGameStateAthena->CurrentPlaylistInfo.OverridePlaylist = Playlist;
			FortGameStateAthena->CurrentPlaylistInfo.PlaylistReplicationKey++;
			FortGameStateAthena->CurrentPlaylistInfo.MarkArrayDirty();

			FortGameStateAthena->OnRep_CurrentPlaylistInfo();
		}
	}

	FortGameModeAthena->WarmupRequiredPlayerCount = 1;
}

APawn* FortGameModeAthena::SpawnDefaultPawnFor_Implementation(AFortGameModeAthena* FortGameModeAthena, AController* NewPlayer, AActor* StartSpot)
{
	static APawn* (*SpawnDefaultPawnFor)(AFortGameModeAthena*, AController*, AActor*) = decltype(SpawnDefaultPawnFor)(AFortGameMode::GetDefaultObj()->VTable[0x610 / 8]);
	return SpawnDefaultPawnFor(FortGameModeAthena, NewPlayer, StartSpot);
}

void FortGameModeAthena::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0xC9FA50, FinishWorldInitialization, (void**)&Originals::FinishWorldInitialization);
	Utils::Hook(InSDKUtils::GetImageBase() + 0xCA8320, InitGameState, (void**)&Originals::InitGameState);

	Utils::Hook(InSDKUtils::GetImageBase() + 0xCB8C30, SpawnDefaultPawnFor_Implementation);
}