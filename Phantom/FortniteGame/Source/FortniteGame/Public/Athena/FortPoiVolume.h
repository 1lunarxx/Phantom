#pragma once
#include "pch.h"
#include "Utils.h"
 
class FortPoiVolume
{
	class Originals
	{
	public:
		static inline bool (*OverlapsPawn)(AFortPoiVolume* FortPoiVolume, AFortPlayerPawn* InPawn);
	};

private:
	static bool OverlapsPawn(AFortPoiVolume* FortPoiVolume, AFortPlayerPawn* InPawn);
public:
	static void Setup();
};