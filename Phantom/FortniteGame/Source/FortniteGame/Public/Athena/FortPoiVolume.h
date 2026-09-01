#pragma once
#include "pch.h"
#include "Utils.h"
 
class FortPoiVolume
{
private:
	static void CheckPawnOverlap(AFortPoiVolume* FortPoiVolume, AFortPlayerPawn* InPawn);
public:
	static void Setup();
};