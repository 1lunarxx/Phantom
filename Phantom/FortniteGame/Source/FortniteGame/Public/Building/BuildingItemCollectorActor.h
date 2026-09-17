#pragma once
#include "pch.h"
#include "Utils.h"

class BuildingItemCollectorActor
{
	class Originals
	{
	public:
		static inline void (*FinishItemSelectionSetup)(ABuildingItemCollectorActor* BuildingItemCollectorActor);
	};

private:
	static void FinishItemSelectionSetup(ABuildingItemCollectorActor* BuildingItemCollectorActor);
	static void OnReachedDepositGoal(ABuildingItemCollectorActor* BuildingItemCollectorActor, AFortPlayerController* CurrentControllingPlayer, UFortWorldItemDefinition* CurrentActiveInputItem);
public:
	static void Setup();
};