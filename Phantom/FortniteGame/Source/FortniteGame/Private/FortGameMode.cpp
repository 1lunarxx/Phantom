#include "pch.h"
#include "Configuration.h"
#include "FortniteGame/Public/FortGameMode.h"

TSubclassOf<AGameSession>* FortGameMode::GetGameSessionClass(AFortGameMode* FortGameMode, TSubclassOf<AGameSession>* Result)
{
    *Result = AFortGameSessionDedicated::StaticClass();
    return Result;
}

// il fix later this is just temp

bool FortGameMode::CanServerTravel(AFortGameMode* FortGameMode, FString* URL, bool bAbsolute)
{
    FString MapPart;
    FString Options;

    FString Ins = L"?";

    if (URL->Split(&Ins, &MapPart, &Options, 0, 0))
    {
        return Originals::CanServerTravel(FortGameMode, &MapPart, bAbsolute);
    }

    return Originals::CanServerTravel(FortGameMode, URL, bAbsolute);
}

void FortGameMode::Setup()
{
    if (Configuration::bIsGameSessionsEnabled)
        Utils::Virtual(AFortGameMode::GetDefaultObj(), 0x668 / 8, GetGameSessionClass);

    Utils::Hook(InSDKUtils::GetImageBase() + 0x2684BF0, CanServerTravel, (void**)&Originals::CanServerTravel);
}