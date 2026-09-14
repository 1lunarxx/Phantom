#pragma once
#include "pch.h"

class UFortLootTier
{
public:
	static bool GetNumLootPackageDropsPerCategory(FFortLootTierData* LootTierData, int32 NumLootPackageDrops, TMap<int32, int32>* OutNumLootPackageDropsPerCategory);
};