#pragma once
#include "pch.h"
#include "Utils.h"

class FortAthenaSupplyDrop
{
private:
	static AFortPickup* SpawnPickup(AFortAthenaSupplyDrop* FortAthenaSupplyDrop, FFrame& Stack, AFortPickup** Ret);
public:
	static void Setup();
};