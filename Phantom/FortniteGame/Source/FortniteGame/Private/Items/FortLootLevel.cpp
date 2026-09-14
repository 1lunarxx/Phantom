#include "pch.h"
#include "FortniteGame/Public/Items/FortLootLevel.h"

__int64 UFortLootLevel::GetItemLevel(FDataTableCategoryHandle* LootLevelData, int LootLevel)
{
    static __int64 (*GetItemLevel)(FDataTableCategoryHandle*, int) = decltype(GetItemLevel)(InSDKUtils::GetImageBase() + 0x1094340);
    return GetItemLevel(LootLevelData, LootLevel);
}