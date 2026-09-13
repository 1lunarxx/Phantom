#pragma once
#include "pch.h"
#include "Utils.h"

class FortAthenaSupplyDrop
{
private:
	static AFortPickup* SpawnPickup(AFortAthenaSupplyDrop* Context, FFrame* Stack, AFortPickup** Result);
public:
	static void Setup();
};