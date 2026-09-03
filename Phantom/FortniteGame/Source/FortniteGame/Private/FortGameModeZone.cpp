#include "pch.h"
#include "FortniteGame/Public/FortGameModeZone.h"

void FortGameModeZone::CreateAIDirector(AFortGameModeZone* FortGameModeZone)
{
	FortGameModeZone->AIDirector = Utils::SpawnActor<AFortAIDirector>(FVector(), FRotator(), AFortAIDirector::StaticClass(), FortGameModeZone);
	FortGameModeZone->AIDirector->Activate();
}

void FortGameModeZone::FinishWorldInitialization(AFortGameModeZone* FortGameModeZone, AFortWorldManager* WorldManager)
{
	if (!FortGameModeZone->bWorldIsReady)
		FortGameModeZone->bWorldIsReady = true;

	WorldManager->WorldManagerState = EFortWorldManagerState::WMS_Running;

	SetConsoleTitleA("Phantom | Ready");
}

void FortGameModeZone::Setup()
{
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xC98E3B, CreateAIDirector);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0x134F889, FinishWorldInitialization);
}