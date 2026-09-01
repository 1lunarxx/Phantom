#pragma once
#include "pch.h"

class FortLootTier
{
public:
	static bool GetNumLootPackageDropsPerCategory(FFortLootTierData* LootTierData, int32 NumLootPackageDrops, TMap<int32, int32>* OutNumLootPackageDropsPerCategory);
};