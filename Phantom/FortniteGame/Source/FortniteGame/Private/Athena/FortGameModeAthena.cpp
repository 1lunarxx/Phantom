#include "pch.h"
#include "FortniteGame/Public/Athena/FortGameModeAthena.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

void FortGameModeAthena::InitGameState(AFortGameModeAthena* FortGameModeAthena)
{
	Originals::InitGameState(FortGameModeAthena);

	if (UFortPlaylistAthena* Playlist = FortGameModeAthena->PlaylistManager->GetAthenaPlaylist(FName(L"Playlist_Barrier_12")))
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

				ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(UWorld::GetWorld(), AdditionalLevel, FVector(), FRotator(), &bSuccess);

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
	if (AFortPlayerControllerAthena* FortPlayerController = Cast<AFortPlayerControllerAthena>(NewPlayer))
	{
		if (AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory())
		{
			for (const FItemAndCount& StartingItem : FortGameModeAthena->StartingItems)
				WorldInventory->AddItem(StartingItem.Item, StartingItem.Count);

			WorldInventory->AddItem(FortPlayerController->CustomizationLoadout.Pickaxe->WeaponDefinition, 1);
		}
	}

	return FortGameModeAthena->SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}

void FortGameModeAthena::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0xCA8320, InitGameState, (void**)&Originals::InitGameState);
	Utils::Hook(InSDKUtils::GetImageBase() + 0xCB8C30, SpawnDefaultPawnFor_Implementation);
}