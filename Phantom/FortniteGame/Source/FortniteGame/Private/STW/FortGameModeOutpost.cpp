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

	if (!UFortGlobals::IsInAthena(GWorld))
	{
		if (AFortGameStateZone* FortGameStateZone = Cast<AFortGameStateZone>(FortGameModeOutpost->GameState))
		{
			UFortGameData* GameData = UFortGameData::Get();

			if (FortGameStateZone->MissionManager == NULL)
				FortGameStateZone->MissionManager = GWorld->SpawnActor<AFortMissionManager>(FVector(), FRotator(), FortGameModeOutpost->MissionManagerClass.Get(), FortGameStateZone);

			FortGameStateZone->MissionManager->BluGloManager = GWorld->SpawnActor<AFortBluGloManager>(FVector(), FRotator(), GameData->BluGloManagerClass.Get(), FortGameStateZone->MissionManager);
			FortGameStateZone->OnRep_MissionManager();

			FortGameModeOutpost->MissionGenerationManager = GWorld->SpawnActor<AFortMissionGenerationManager>(FVector(), FRotator(), AFortMissionGenerationManager::StaticClass(), FortGameModeOutpost);
		}
	}
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
	APawn* DefaultPawn = FortGameModeOutpost->SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	if (AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(NewPlayer))
	{
		if (FortPlayerController->QuickBars == NULL)
			FortPlayerController->QuickBars = GWorld->SpawnActor<AFortQuickBars>(FVector(), FRotator(), AFortQuickBars::StaticClass(), FortPlayerController);

		if (AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory())
		{
			// stw doesnt want startingitems so im forced to do this!!!!

			if (FortGameModeOutpost->StartingItems.Num() <= 0)
			{
				UFortBuildingItemDefinition* BuildingItemData_Wall = Utils::StaticFindObject<UFortBuildingItemDefinition>(TEXT("BuildingItemData_Wall"), ANY_PACKAGE);
				UFortBuildingItemDefinition* BuildingItemData_Floor = Utils::StaticFindObject<UFortBuildingItemDefinition>(TEXT("BuildingItemData_Floor"), ANY_PACKAGE);
				UFortBuildingItemDefinition* BuildingItemData_Stair_W = Utils::StaticFindObject<UFortBuildingItemDefinition>(TEXT("BuildingItemData_Stair_W"), ANY_PACKAGE);
				UFortBuildingItemDefinition* BuildingItemData_RoofS = Utils::StaticFindObject<UFortBuildingItemDefinition>(TEXT("BuildingItemData_RoofS"), ANY_PACKAGE);
				UFortEditToolItemDefinition* EditToolItemDefinition = Utils::StaticFindObject<UFortEditToolItemDefinition>(TEXT("EditTool"), ANY_PACKAGE);

				WorldInventory->AddItem(BuildingItemData_Wall, 1);
				WorldInventory->AddItem(BuildingItemData_Floor, 1);
				WorldInventory->AddItem(BuildingItemData_Stair_W, 1);
				WorldInventory->AddItem(BuildingItemData_RoofS, 1);
				WorldInventory->AddItem(EditToolItemDefinition, 1);
			}

			UAthenaPickaxeItemDefinition* AthenaPickaxeItemDefinition = Utils::StaticFindObject<UAthenaPickaxeItemDefinition>(TEXT("DefaultPickaxe"), ANY_PACKAGE);

			if (AthenaPickaxeItemDefinition != NULL)
				WorldInventory->AddItem(AthenaPickaxeItemDefinition->WeaponDefinition, 1);
		}
	}

	return DefaultPawn;
}

void FortGameModeOutpost::Setup()
{
	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0xC08 / 8, CreateAIDirector);
	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0x610 / 8, SpawnDefaultPawnFor_Implementation);

	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0xA40 / 8, FinishWorldInitialization, (void**)&Originals::FinishWorldInitialization);
	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0x660 / 8, InitGameState, (void**)&Originals::InitGameState);
}