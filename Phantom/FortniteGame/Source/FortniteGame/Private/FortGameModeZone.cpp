#include "pch.h"
#include "FortniteGame/Public/FortGameModeZone.h"

void FortGameModeZone::CreateAIDirector(AFortGameModeZone* FortGameModeZone)
{
	UClass* AIDirectorClass = AFortAIDirector::StaticClass();

	if (UFortGameData::Get()->AIDirectors.IsValidIndex(FortGameModeZone->OverrideAIDirectorIndex))
		AIDirectorClass = UFortGameData::Get()->AIDirectors[FortGameModeZone->OverrideAIDirectorIndex].Get();

	FortGameModeZone->AIDirector = GWorld->SpawnActor<AFortAIDirector>(FVector(), FRotator(), AIDirectorClass, FortGameModeZone);
	FortGameModeZone->AIDirector->Activate();
}

void FortGameModeZone::FinishWorldInitialization(AFortGameModeZone* FortGameModeZone, AFortWorldManager* WorldManager)
{
	if (WorldManager != NULL)
		WorldManager->FinishWorldInitialization();

	FortGameModeZone->FinishWorldInitialization(WorldManager);

	SetConsoleTitleA("Phantom | Ready");
}

APawn* FortGameModeZone::SpawnDefaultPawnFor_Implementation(AFortGameModeZone* FortGameModeZone, AController* NewPlayer, AActor* StartSpot)
{
	APawn* DefaultPawn = FortGameModeZone->SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	if (AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(NewPlayer))
	{
		if (FortPlayerController->QuickBars == NULL)
			FortPlayerController->QuickBars = GWorld->SpawnActor<AFortQuickBars>(FVector(), FRotator(), AFortQuickBars::StaticClass(), FortPlayerController);

		if (AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory())
		{
			// stw doesnt want startingitems so im forced to do this!!!!

			if (FortGameModeZone->StartingItems.Num() <= 0)
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

void FortGameModeZone::Setup()
{
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xC98E3B, CreateAIDirector);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0x134F889, FinishWorldInitialization);

	Utils::Virtual(AFortGameModeZone::GetDefaultObj()->VTable, 0xC08 / 8, CreateAIDirector);
	Utils::Virtual(AFortGameModeZone::GetDefaultObj()->VTable, 0xA40 / 8, FinishWorldInitialization);
	Utils::Virtual(AFortGameModeZone::GetDefaultObj()->VTable, 0x610 / 8, SpawnDefaultPawnFor_Implementation);
}