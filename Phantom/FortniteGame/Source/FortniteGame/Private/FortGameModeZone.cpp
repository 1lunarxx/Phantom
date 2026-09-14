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
	return FortGameModeZone->SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}

void FortGameModeZone::Setup()
{
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xC98E3B, CreateAIDirector);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0x134F889, FinishWorldInitialization);

	Utils::Virtual(AFortGameModeZone::GetDefaultObj()->VTable, 0xC08 / 8, CreateAIDirector);
	Utils::Virtual(AFortGameModeZone::GetDefaultObj()->VTable, 0xA40 / 8, FinishWorldInitialization);
	Utils::Virtual(AFortGameModeZone::GetDefaultObj()->VTable, 0x610 / 8, SpawnDefaultPawnFor_Implementation);
}