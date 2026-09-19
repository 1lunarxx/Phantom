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
/*	FortGameModeZone->AIDirector->Activate();*/
}

void FortGameModeZone::FinishWorldInitialization(AFortGameModeZone* FortGameModeZone, AFortWorldManager* WorldManager)
{
	FortGameModeZone->FinishWorldInitialization(WorldManager);

/*	if (AFortGameStateZone* FortGameStateZone = Cast<AFortGameStateZone>(FortGameModeZone->GameState))
	{
		if (FortGameStateZone->MissionManager == NULL)
			FortGameStateZone->CreateMissionManager(&WorldManager->MissionManagerRecord);
	}*/

	SetConsoleTitleA("Phantom | Ready");
}

APawn* FortGameModeZone::SpawnDefaultPawnFor_Implementation(AFortGameModeZone* FortGameModeZone, AController* NewPlayer, AActor* StartSpot)
{
	FTransform Transform = StartSpot->GetTransform();
	Transform.Translation.Z += 1000; // yes im scuffed im sorry this is temp

	if (AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(NewPlayer))
	{
		if (FortPlayerController->QuickBars == NULL)
		{
			FortPlayerController->SpawnQuickBars();
			FortPlayerController->SetupQuickBars();
		}
	}

	return FortGameModeZone->SpawnDefaultPawnAtTransform(NewPlayer, Transform);
}

void FortGameModeZone::Setup()
{
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xC98E3B, CreateAIDirector);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0x134F889, FinishWorldInitialization);

	Utils::Virtual<AFortGameModeZone, AFortGameModeOutpost, AFortGameModePvE>(0xC08 / 8, CreateAIDirector);
	Utils::Virtual<AFortGameModeZone, AFortGameModeOutpost, AFortGameModePvE>(0xA40 / 8, FinishWorldInitialization);
	Utils::Virtual<AFortGameModeZone, AFortGameModeOutpost, AFortGameModePvE>(0x610 / 8, SpawnDefaultPawnFor_Implementation);
}