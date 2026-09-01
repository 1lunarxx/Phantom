#pragma once
#include "pch.h"
#include "Utils.h"

class FortLootPackage
{
public:
    static FFortLootTierData* FindLootTierDataRow(UDataTable* DataTable, FName RowName, const TCHAR* ContextString, bool bWarnIfRowMissing)
    {
        if (RowName.IsNone())
            return NULL;

        float TotalWeight = 0.0f;

        TArray<FFortLootTierData*> FortLootTierDataArray;
        for (const auto& [RowName_Map, RowValue_Map] : DataTable->RowMap)
        {
            FFortLootTierData* FortLootTierData = reinterpret_cast<FFortLootTierData*>(RowValue_Map);

            if (FortLootTierData == NULL || FortLootTierData->TierGroup != RowName || FortLootTierData->Weight <= 0.0f)
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
	static void PickLootDrops(TArray<FFortItemEntry>* OutLootToDrop, int ForcedLootTier, FName TierGroupName);
	static void PickLootDropsFromLootPackage(TArray<FFortItemEntry>* OutLootToDrop, FName LootPackage, int32 ForcedLootTier, int32 LootPackageCategory, int32 WorldLevel);
};