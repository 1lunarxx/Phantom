#include "pch.h"
#include "FortniteGame/Public/FortKismetLibrary.h"

AFortAIGoalManager* FortKismetLibrary::GetAIGoalManager(UObject* WorldContextObject)
{
    return GGameMode->AIGoalManager;
}

void FortKismetLibrary::Setup()
{
    Utils::Rel32(InSDKUtils::GetImageBase() + 0x159CD08, GetAIGoalManager);
}