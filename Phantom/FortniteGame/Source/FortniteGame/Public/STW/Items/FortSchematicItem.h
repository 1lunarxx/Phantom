#pragma once
#include "pch.h"
#include "Utils.h"

class FortSchematicItem
{
private:
	static int32 ServerCraftSchematic(UFortSchematicItem* FortSchematicItem, AFortPlayerController* Instigator, int32 PostCraftSlot, int32 CraftAmount, EFortItemTier RequestedTier);
public:
	static void Setup();
};