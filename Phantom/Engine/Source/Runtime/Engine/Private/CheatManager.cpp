#include "pch.h"
#include "GameFramework/CheatManager.h"

void CheatManager::Slomo(UCheatManager* CheatManager, float NewTimeDilation)
{
	APlayerController* PlayerController = Cast<APlayerController>(CheatManager->Outer);

	if (PlayerController != NULL)
		PlayerController->GetWorldSettings()->TimeDilation = NewTimeDilation;
}

void CheatManager::Setup()
{
	Utils::Virtual(UCheatManager::GetDefaultObj()->VTable, 0x280 / 8, Slomo);
}