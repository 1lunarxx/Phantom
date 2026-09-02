#include "pch.h"
#include "FortniteGame/Public/Athena/FortGameModeAthena.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

void FortGameModeAthena::FinishWorldInitialization(AFortGameModeAthena* FortGameModeAthena, AFortWorldManager* WorldManager)
{
	UClass* Tiered_Athena_FloorLoot = Utils::StaticLoadObject<UClass>(TEXT("/Game/Athena/Environments/Blueprints/Tiered_Athena_FloorLoot_01.Tiered_Athena_FloorLoot_01_C"));
	UClass* Tiered_Athena_FloorLoot_Warmup = Utils::StaticLoadObject<UClass>(TEXT("/Game/Athena/Environments/Blueprints/Tiered_Athena_FloorLoot_Warmup.Tiered_Athena_FloorLoot_Warmup_C"));

	if (Tiered_Athena_FloorLoot != NULL)
	{
		for (ABuildingContainer* BuildingContainer : Utils::GetAllActors<ABuildingContainer>(Tiered_Athena_FloorLoot))
			BuildingContainer->SpawnLoot(NULL);
	}

	if (Tiered_Athena_FloorLoot_Warmup != NULL)
	{
		for (ABuildingContainer* BuildingContainer : Utils::GetAllActors<ABuildingContainer>(Tiered_Athena_FloorLoot_Warmup))
			BuildingContainer->SpawnLoot(NULL);
	}

	SetConsoleTitleA("Phantom | Ready");

	FortGameModeAthena->bWorldIsReady = true;

	Originals::FinishWorldInitialization(FortGameModeAthena, WorldManager);
}

void FortGameModeAthena::InitGameState(AFortGameModeAthena* FortGameModeAthena)
{
	Originals::InitGameState(FortGameModeAthena);

/*	if (UFortPlaylistAthena* Playlist = FortGameModeAthena->PlaylistManager->GetAthenaPlaylist(FName(L"Playlist_Deimos_50v50")))
	{*/
	if (UFortPlaylistAthena* Playlist = Utils::StaticFindObject<UFortPlaylistAthena>(TEXT("Playlist_Deimos_50v50"), ANY_PACKAGE))
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
		}

		for (TSoftObjectPtr<UWorld>& AdditionalLevel : Playlist->AdditionalLevels)
		{
			bool bSuccess = false;
			ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(UWorld::GetWorld(), AdditionalLevel, FVector(), FRotator(), &bSuccess);

			if (bSuccess)
				GGameState->AdditionalPlaylistLevelsStreamed.Add(AdditionalLevel.ObjectID.AssetPathName);
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
	Utils::Hook(InSDKUtils::GetImageBase() + 0xC9FA50, FinishWorldInitialization, (void**)&Originals::FinishWorldInitialization);
	Utils::Hook(InSDKUtils::GetImageBase() + 0xCA8320, InitGameState, (void**)&Originals::InitGameState);

	Utils::Hook(InSDKUtils::GetImageBase() + 0xCB8C30, SpawnDefaultPawnFor_Implementation);
}