#include "pch.h"
#include "FortniteGame/Public/Athena/FortGameModeAthena.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

// Playlists full working: Playlist_Disco_32/Playlist_Disco_32_Alt, Playlist_DefaultSolo, Playlist_HighExplosives_Solo, Playlist_Playground, Playlist_WW_Solo, Playlist_50v50
// Playlists half working: Playlist_Barrier/Playlist_Barrier_12
// Playlists todo: Playlist_Deimos_50, Playlist_Soaring_50s, Playlist_Fortnite

void FortGameModeAthena::InitGameState(AFortGameModeAthena* FortGameModeAthena)
{
	Originals::InitGameState(FortGameModeAthena);

	if (UFortPlaylistAthena* Playlist = FortGameModeAthena->PlaylistManager->GetAthenaPlaylist(FName(L"Playlist_DefaultSolo")))
	{
		FortGameModeAthena->CurrentPlaylistId = Playlist->PlaylistId;
		FortGameModeAthena->CurrentPlaylistName = Playlist->PlaylistName;

		FortGameModeAthena->GameSession->MaxPlayers = Playlist->MaxPlayers;
		FortGameModeAthena->AISettings = Playlist->AISettings;

		if (FortGameModeAthena->AIGoalManager == NULL)
			FortGameModeAthena->CreateAIGoalManager();

		if (AFortGameStateAthena* FortGameStateAthena = Cast<AFortGameStateAthena>(FortGameModeAthena->GameState))
		{
			FortGameStateAthena->CurrentPlaylistId = Playlist->PlaylistId;

			FortGameStateAthena->CurrentPlaylistInfo.BasePlaylist = Playlist;
			FortGameStateAthena->CurrentPlaylistInfo.OverridePlaylist = Playlist;
			FortGameStateAthena->CurrentPlaylistInfo.PlaylistReplicationKey++;
			FortGameStateAthena->CurrentPlaylistInfo.MarkArrayDirty();

			FortGameStateAthena->OnRep_CurrentPlaylistInfo();

			FortGameStateAthena->AirCraftBehavior = Playlist->AirCraftBehavior;
			FortGameStateAthena->CachedSafeZoneStartUp = Playlist->SafeZoneStartUp;

			for (TSoftObjectPtr<UWorld>& AdditionalLevel : Playlist->AdditionalLevels)
			{
				bool bSuccess = false;

				ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GWorld, AdditionalLevel, FVector(), FRotator(), &bSuccess);

				if (bSuccess)
					FortGameStateAthena->AdditionalPlaylistLevelsStreamed.Add(AdditionalLevel.ObjectID.AssetPathName);
			}

			FortGameStateAthena->OnRep_AdditionalPlaylistLevelsStreamed();
		}
	}

	FortGameModeAthena->WarmupRequiredPlayerCount = 1;
}

APawn* FortGameModeAthena::SpawnDefaultPawnFor_Implementation(AFortGameModeAthena* FortGameModeAthena, AController* NewPlayer, AActor* StartSpot)
{
	APawn* DefaultPawn = FortGameModeAthena->SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);;

	if (AFortPlayerControllerAthena* FortPlayerController = Cast<AFortPlayerControllerAthena>(NewPlayer))
	{
		if (AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory())
		{
			TArray<FItemAndCount> StartingItems;
			FortGameModeAthena->GetStartingItems(&StartingItems, !FortPlayerController->bHasInitiallySpawned, NewPlayer);

			for (const FItemAndCount& StartingItem : StartingItems)
				WorldInventory->AddItem(StartingItem.Item, StartingItem.Count);

			WorldInventory->AddItem(FortPlayerController->CustomizationLoadout.Pickaxe->WeaponDefinition, 1);
		}
	}

	return DefaultPawn;
}

void FortGameModeAthena::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0xCA8320, InitGameState, (void**)&Originals::InitGameState);
	Utils::Hook(InSDKUtils::GetImageBase() + 0xCB8C30, SpawnDefaultPawnFor_Implementation);
}