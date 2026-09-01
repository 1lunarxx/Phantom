#include "pch.h"
#include "FortniteGame/Public/Athena/FortPoiVolume.h"

void FortPoiVolume::CheckPawnOverlap(AFortPoiVolume* FortPoiVolume, AFortPlayerPawn* InPawn)
{
}

void FortPoiVolume::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0xCFD880, CheckPawnOverlap);
}