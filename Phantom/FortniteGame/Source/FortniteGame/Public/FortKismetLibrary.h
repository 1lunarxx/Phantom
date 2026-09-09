#pragma once
#include "pch.h"
#include "Utils.h"

class FortKismetLibrary
{
private:
	static AFortAIGoalManager* GetAIGoalManager(UFortKismetLibrary* FortKismetLibrary, FFrame& Stack, AFortAIGoalManager** Ret);
	static AFortPickup* K2_SpawnPickupInWorld(UFortKismetLibrary* FortKismetLibrary, FFrame& Stack, AFortPickup** Ret);

	static bool PickLootDrops(UFortKismetLibrary* FortKismetLibrary, FFrame& Stack, bool* Ret);
public:
	static void Setup();
};