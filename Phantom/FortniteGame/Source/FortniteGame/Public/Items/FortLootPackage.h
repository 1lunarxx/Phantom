#pragma once
#include "pch.h"
#include "Utils.h"

class UFortLootPackage
{
public:
    static FFortLootTierData* FindLootTierDataRow(UDataTable* DataTable, FName RowName, const TCHAR* ContextString, bool bWarnIfRowMissing, int ForcedLootTier)
    {
        if (RowName.IsNone())
            return NULL;

        float TotalWeight = 0.0f;

        TArray<FFortLootTierData*> FortLootTierDataArray;

        for (const auto& [RowName_, RowValue] : DataTable->RowMap)
        {
            FFortLootTierData* FortLootTierData = (FFortLootTierData*)RowValue;

            if (FortLootTierData == NULL)
                continue;

            if (FortLootTierData->TierGroup != RowName)
                continue;

            if (FortLootTierData->Weight <= 0.0f)
                continue;

            if (ForcedLootTier != -1 && FortLootTierData->LootTier != ForcedLootTier)
                continue;

            FortLootTierDataArray.Add(FortLootTierData);
            TotalWeight += FortLootTierData->Weight;
        }

        if (FortLootTierDataArray.Num() <= 0 || TotalWeight <= 0.0f)
            return NULL;

        float RandomWeight = UKismetMathLibrary::RandomFloat() * TotalWeight;

        for (FFortLootTierData* FortLootTierData : FortLootTierDataArray)
        {
            if (RandomWeight <= FortLootTierData->Weight)
                return FortLootTierData;

            RandomWeight -= FortLootTierData->Weight;
        }

        return NULL;
    }
public:
	static UDataTable* GetLootTierData();
	static UDataTable* GetLootPackageData();
public:
	static bool PickLootDrops(TArray<FFortItemEntry>* OutLootToDrop, int ForcedLootTier, FName TierGroupName);
	static void PickLootDropsFromLootPackage(TArray<FFortItemEntry>* OutLootToDrop, FName LootPackage, int32 ForcedLootTier, int32 LootPackageCategory, int32 WorldLevel);
};