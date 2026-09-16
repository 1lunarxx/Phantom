#include "pch.h"
#include "FortniteGame/Public/FortGameModeZone.h"
#include "Core/Public/Math/UnrealMathUtility.h"

bool AFortGameModeZone::FindBestStartTransformFor(AController* NewPlayer, const AActor* StartSpot, FTransform* OutBestTransform)
{
	UClass* DefaultPawnClassForController = GetDefaultPawnClassForController(NewPlayer);

	if (DefaultPawnClassForController == NULL)
		return false;

	AFortPlayerPawn* FortPlayerPawn = Cast<AFortPlayerPawn>(DefaultPawnClassForController->DefaultObject);

	if (FortPlayerPawn == NULL)
		return false;

	if (PlayerStartLocationOverrides.Num() > 0)
	{
		*OutBestTransform = UKismetMathLibrary::MakeTransform(PlayerStartLocationOverrides[FMath::Rand() % PlayerStartLocationOverrides.Num()] + FVector(0, 0, FortPlayerPawn->CapsuleComponent->CapsuleHalfHeight + 24.f), FRotator(), FVector(1, 1, 1));
		return true;
	}
	else if (AFortGameModeOutpost* FortGameModeOutpost = Cast<AFortGameModeOutpost>(this))
	{
		FVector Location = StartSpot->K2_GetActorLocation();
		Location.Z += 1000;

		*OutBestTransform = UKismetMathLibrary::MakeTransform(Location, FRotator(), FVector(1, 1, 1));
		return true;
	}

	return false;
}

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
	{
		WorldManager->FinishWorldInitialization();

		if (AFortGameStateZone* FortGameStateZone = Cast<AFortGameStateZone>(FortGameModeZone->GameState))
		{
			// TODO: find pooper MissionGenerator

			// /Game/World/MissionGens/MissionGen_Onboarding_Fort.MissionGen_Onboarding_Fort_C, /Game/World/MissionGens/MissionGen_TheOutpost_PvE_01.MissionGen_TheOutpost_PvE_01_C

			UFortMissionGenerator* MissionGenerator = NewObject<UFortMissionGenerator>(FortGameModeZone, Utils::StaticLoadObject<UClass>(TEXT("/Game/World/MissionGens/MissionGen_Onboarding_Fort.MissionGen_Onboarding_Fort_C")));

			FFortMissionManagerRecord* MissionManagerRecord = &WorldManager->MissionManagerRecord;
			FFortMissionRecord MissionRecord = FFortMissionRecord{};

			MissionRecord.MissionEntry.MissionGenerator = MissionGenerator;
			MissionRecord.MissionEntry.MissionInfo = MissionGenerator->PrimaryMissionInfo.LoadSynchronous();

			MissionManagerRecord->MissionRecords.Add(MissionRecord);

			if (FortGameStateZone->MissionManager == NULL)
				FortGameStateZone->CreateMissionManager(MissionManagerRecord);
		}
	}

	FortGameModeZone->FinishWorldInitialization(WorldManager);
	
	SetConsoleTitleA("Phantom | Ready");
}

void FortGameModeZone::InitGameState(AFortGameModeZone* FortGameModeZone)
{
	Originals::InitGameState(FortGameModeZone);

	if (FortGameModeZone->AIGoalManager == NULL)
		FortGameModeZone->CreateAIGoalManager();

	FortGameModeZone->MissionGenerationManager = GWorld->SpawnActor<AFortMissionGenerationManager>(FVector(), FRotator(), AFortMissionGenerationManager::StaticClass(), FortGameModeZone);
}

void FortGameModeZone::HandleStartingNewPlayer(AFortGameModeZone* FortGameModeZone, APlayerController* NewPlayer)
{
	Originals::HandleStartingNewPlayer(FortGameModeZone, NewPlayer);
}

APawn* FortGameModeZone::SpawnDefaultPawnFor_Implementation(AFortGameModeZone* FortGameModeZone, AController* NewPlayer, AActor* StartSpot)
{
	FTransform Transform;

	if (!FortGameModeZone->FindBestStartTransformFor(NewPlayer, StartSpot, &Transform))
		Transform = StartSpot->GetTransform();
	
	APawn* DefaultPawn = FortGameModeZone->SpawnDefaultPawnAtTransform(NewPlayer, Transform);

	if (AFortPlayerControllerZone* FortPlayerController = Cast<AFortPlayerControllerZone>(NewPlayer))
	{
		if (FortPlayerController->QuickBars == NULL)
			FortPlayerController->QuickBars = GWorld->SpawnActor<AFortQuickBars>(FVector(), FRotator(), AFortQuickBars::StaticClass(), FortPlayerController);

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

	Utils::Virtual(AFortGameModeZone::GetDefaultObj()->VTable, 0x640 / 8, HandleStartingNewPlayer, (void**)&Originals::HandleStartingNewPlayer);

	Utils::Virtual<AFortGameModeZone, AFortGameModeOutpost>(0xC08 / 8, CreateAIDirector);
	Utils::Virtual<AFortGameModeZone, AFortGameModeOutpost>(0xA40 / 8, FinishWorldInitialization);
	Utils::Virtual<AFortGameModeZone, AFortGameModeOutpost>(0x610 / 8, SpawnDefaultPawnFor_Implementation);
	Utils::Virtual<AFortGameModeZone, AFortGameModeOutpost>(0x660 / 8, InitGameState, (void**)&Originals::InitGameState);
}