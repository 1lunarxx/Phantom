#include "pch.h"
#include "FortniteGame/Public/FortGameModeZone.h"

void FortGameModeZone::CreateAIDirector(AFortGameModeZone* FortGameModeZone)
{
	FortGameModeZone->AIDirector = Utils::SpawnActor<AFortAIDirector>(FVector(), FRotator(), AFortAIDirector::StaticClass(), FortGameModeZone);
	FortGameModeZone->AIDirector->Activate();
}

void FortGameModeZone::Setup()
{
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xC98E3B, CreateAIDirector);
}