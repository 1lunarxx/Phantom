#include "pch.h"
#include "FortniteGame/Public/Athena/FortPoiVolume.h"

bool FortPoiVolume::OverlapsPawn(AFortPoiVolume* FortPoiVolume, AFortPlayerPawn* InPawn)
{
    if (FortPoiVolume->BrushComponent == NULL)
        return false;

    return Originals::OverlapsPawn(FortPoiVolume, InPawn);
}

void FortPoiVolume::Setup()
{
    Utils::Hook(InSDKUtils::GetImageBase() + 0xD14090, OverlapsPawn, (void**)&Originals::OverlapsPawn);
}