#pragma once
#include "pch.h"
#include "Utils.h"

class BuildingContainer
{
private:
	static bool SpawnLoot(ABuildingContainer* BuildingContainer, AFortPlayerPawn* PlayerPawn, const EFortPickupSourceTypeFlag InSourceTypeFlag, const uint8 InSpawnSource);
public:
	static void Setup();
};