#pragma once
#include "pch.h"
#include "Utils.h"

class BuildingContainer
{
	class Originals
	{
	public:
		static inline void (*PostUpdate)(ABuildingContainer* BuildingContainer, EFortBuildingPersistentState PersistentState, const void* ReservedRandomValues);
	};

private:
	static void PostUpdate(ABuildingContainer* BuildingContainer, EFortBuildingPersistentState PersistentState, const void* ReservedRandomValues);
	static bool SpawnLoot(ABuildingContainer* BuildingContainer, AFortPlayerPawn* PlayerPawn, const EFortPickupSourceTypeFlag InSourceTypeFlag, const uint8 InSpawnSource);
public:
	static void Setup();
};