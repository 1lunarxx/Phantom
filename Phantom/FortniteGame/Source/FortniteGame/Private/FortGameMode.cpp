#include "pch.h"
#include "Configuration.h"
#include "FortniteGame/Public/FortGameMode.h"

TSubclassOf<AGameSession>* FortGameMode::GetGameSessionClass(AFortGameMode* FortGameMode, TSubclassOf<AGameSession>* Result)
{
    *Result = AFortGameSessionDedicated::StaticClass();
    return Result;
}


void FortGameMode::Setup()
{
    if (Configuration::bIsGameSessionsEnabled)
        Utils::Virtual(AFortGameMode::GetDefaultObj(), 0x668 / 8, GetGameSessionClass);
}