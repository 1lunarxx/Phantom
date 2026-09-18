#include "pch.h"
#include "FortniteGame/Public/FortGameModeZone.h"
#include "Core/Public/Math/UnrealMathUtility.h"

void FortGameModeZone::CreateAIDirector(AFortGameModeZone* FortGameModeZone)
{
	UFortGameData* GameData = UFortGameData::Get();
	UClass* AIDirectorClass = AFortAIDirector::StaticClass();

	if (GameData->AIDirectors.IsValidIndex(FortGameModeZone->OverrideAIDirectorIndex))
		AIDirectorClass = GameData->AIDirectors[FortGameModeZone->OverrideAIDirectorIndex].Get();

	FortGameModeZone->AIDirector = GWorld->SpawnActor<AFortAIDirector>(FVector(), FRotator(), AIDirectorClass, FortGameModeZone);
	FortGameModeZone->AIDirector->Activate();
}

void FortGameModeZone::FinishWorldInitialization(AFortGameModeZone* FortGameModeZone, AFortWorldManager* WorldManager)
{
	if (WorldManager != NULL)
	{
		if (AFortGameStateOutpost* FortGameStateOutpost = Cast<AFortGameStateOutpost>(FortGameModeZone->GameState))
		{
			// scuffed because no gamesessions

			UFortMissionGenerator* MissionGenerator = NewObject<UFortMissionGenerator>(FortGameModeZone, Utils::StaticLoadObject<UClass>(TEXT("/Game/World/MissionGens/MissionGen_TheOutpost_PvE_01.MissionGen_TheOutpost_PvE_01_C")));

			FFortMissionManagerRecord MissionManagerRecord = WorldManager->MissionManagerRecord;
			FFortMissionRecord MissionRecord = FFortMissionRecord{};

			MissionRecord.MissionEntry.MissionGenerator = MissionGenerator;
			MissionRecord.MissionEntry.MissionInfo = MissionGenerator->PrimaryMissionInfo.LoadSynchronous();

			MissionManagerRecord.MissionRecords.Add(MissionRecord);

			if (FortGameStateOutpost->MissionManager == NULL)
				FortGameStateOutpost->CreateMissionManager(&MissionManagerRecord);

			if (FortGameModeZone->AIGoalManager == NULL)
				FortGameModeZone->CreateAIGoalManager();

			FortGameModeZone->MissionGenerationManager = GWorld->SpawnActor<AFortMissionGenerationManager>(FVector(), FRotator(), AFortMissionGenerationManager::StaticClass(), FortGameModeZone);
		}
	}

	FortGameModeZone->FinishWorldInitialization(WorldManager);
	
	SetConsoleTitleA("Phantom | Ready");
}

APawn* FortGameModeZone::SpawnDefaultPawnFor_Implementation(AFortGameModeZone* FortGameModeZone, AController* NewPlayer, AActor* StartSpot)
{
	APawn* DefaultPawn = FortGameModeZone->SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	if (AFortPlayerControllerZone* FortPlayerController = Cast<AFortPlayerControllerZone>(NewPlayer))
	{
		if (FortPlayerController->QuickBars == NULL)
		{
			printf(__FUNCTION__);
			FortPlayerController->QuickBars = GWorld->SpawnActor<AFortQuickBars>(FVector(), FRotator(), AFortQuickBars::StaticClass(), FortPlayerController);
		}

		if (AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory())
		{
			for (const FItemDefinitionAndCount& InventoryItem : UFortGameData::Get()->FastLoadDefaultInventoryList)
			{
				WorldInventory->AddItem(InventoryItem.ItemDefinition.LoadSynchronous(), InventoryItem.Count);
			}
		}
	}

	return DefaultPawn;
}

void FortGameModeZone::Setup()
{
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xC98E3B, CreateAIDirector);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0x134F889, FinishWorldInitialization);

	Utils::Virtual<AFortGameModeZone, AFortGameModeOutpost>(0xC08 / 8, CreateAIDirector);
	Utils::Virtual<AFortGameModeZone, AFortGameModeOutpost>(0xA40 / 8, FinishWorldInitialization);
	Utils::Virtual<AFortGameModeZone, AFortGameModeOutpost>(0x610 / 8, SpawnDefaultPawnFor_Implementation);
}