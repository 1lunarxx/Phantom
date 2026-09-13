#pragma once
#include "pch.h"
#include "Utils.h"

class FortKismetLibrary
{
private:
	static AFortAIGoalManager* GetAIGoalManager(UFortKismetLibrary* Context, FFrame* Stack, AFortAIGoalManager** Result);
	static AFortPickup* K2_SpawnPickupInWorld(UFortKismetLibrary* Context, FFrame* Stack, AFortPickup** Result);

	static bool PickLootDrops(UFortKismetLibrary* Context, FFrame* Stack, bool* Result);
public:
	static void Setup();
};