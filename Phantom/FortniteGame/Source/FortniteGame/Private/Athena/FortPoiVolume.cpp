#include "pch.h"
#include "FortniteGame/Public/Athena/FortPoiVolume.h"

bool FortPoiVolume::OverlapsPawn(AFortPoiVolume* FortPoiVolume, AFortPlayerPawn* InPawn)
{
    return true;
}

void FortPoiVolume::Setup()
{
    Utils::Hook(InSDKUtils::GetImageBase() + 0xD14090, OverlapsPawn);
}