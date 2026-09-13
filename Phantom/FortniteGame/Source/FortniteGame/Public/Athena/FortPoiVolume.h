#pragma once
#include "pch.h"
#include "Utils.h"
 
class FortPoiVolume
{
private:
	static bool OverlapsPawn(AFortPoiVolume* FortPoiVolume, AFortPlayerPawn* InPawn);
public:
	static void Setup();
};