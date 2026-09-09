#include "pch.h"
#include "FortniteGame/Public/FortGameModeZone.h"

void FortGameModeZone::CreateAIDirector(AFortGameModeZone* FortGameModeZone)
{
	FortGameModeZone->AIDirector = GWorld->SpawnActor<AFortAIDirector>(FVector(), FRotator(), AFortAIDirector::StaticClass(), FortGameModeZone);
	FortGameModeZone->AIDirector->Activate();
}

void FortGameModeZone::FinishWorldInitialization(AFortGameModeZone* FortGameModeZone, AFortWorldManager* WorldManager)
{
	if (WorldManager != NULL)
		WorldManager->FinishWorldInitialization();

	FortGameModeZone->FinishWorldInitialization(WorldManager);

	SetConsoleTitleA("Phantom | Ready");
}

void FortGameModeZone::HandleStartingNewPlayer_Implementation(AFortGameModeZone* FortGameModeZone, AFortPlayerController* NewPlayer)
{
	Originals::HandleStartingNewPlayer_Implementation(FortGameModeZone, NewPlayer);

	if (AFortPlayerControllerAthena* FortPlayerControllerAthena = Cast<AFortPlayerControllerAthena>(NewPlayer))
	{
		if (FortPlayerControllerAthena->MatchReport == NULL)
			FortPlayerControllerAthena->MatchReport = NewObject<UAthenaPlayerMatchReport>(FortPlayerControllerAthena);
	}
}

void FortGameModeZone::Setup()
{
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xC98E3B, CreateAIDirector);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0x134F889, FinishWorldInitialization);

	Utils::Virtual(AFortGameModeAthena::GetDefaultObj()->VTable, 0x640 / 8, HandleStartingNewPlayer_Implementation, (void**)&Originals::HandleStartingNewPlayer_Implementation);
}