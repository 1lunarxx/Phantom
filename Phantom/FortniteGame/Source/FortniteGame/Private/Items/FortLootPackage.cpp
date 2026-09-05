#include "pch.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"
#include "FortniteGame/Public/Items/FortLootTier.h"
#include "Core/Public/Math/UnrealMathUtility.h"

UDataTable* FortLootPackage::GetLootTierData()
{
	UFortPlaylistAthena* Playlist = GGameState->CurrentPlaylistInfo.BasePlaylist;

	if (Playlist != NULL)
	{
		UDataTable* LootTierData = Playlist->LootTierData.Get();

		if (LootTierData != NULL)
			return LootTierData;
	}

	return Utils::StaticLoadObject<UDataTable>(TEXT("/Game/Items/DataTables/AthenaLootTierData_Client.AthenaLootTierData_Client"));
}

UDataTable* FortLootPackage::GetLootPackageData()
{
	UFortPlaylistAthena* Playlist = GGameState->CurrentPlaylistInfo.BasePlaylist;

	if (Playlist != NULL)
	{
		UDataTable* LootPackageData = Playlist->LootPackages.Get();

		if (LootPackageData != NULL)
			return LootPackageData;
	}

	return Utils::StaticLoadObject<UDataTable>(TEXT("/Game/Items/DataTables/AthenaLootPackages_Client.AthenaLootPackages_Client"));
}

void FortLootPackage::PickLootDrops(TArray<FFortItemEntry>* OutLootToDrop, int ForcedLootTier, FName TierGroupName)
{
	if (OutLootToDrop == NULL)
		return;

	UDataTable* LootTierData = GetLootTierData();
	UDataTable* LootPackageData = GetLootPackageData();

	if (LootTierData == NULL || LootPackageData == NULL)
		return;

	FFortLootTierData* FortLootTierData = FindLootTierDataRow(LootTierData, TierGroupName, TEXT("UFortLootPackage::PickLootDrops"), true);

	if (FortLootTierData == NULL)
		return;

	FName LootPackage = FortLootTierData->LootPackage;

	if (LootPackage.IsNone())
		return;

	int32 NumLootPackageDrops = (int32)FortLootTierData->NumLootPackageDrops;

	if (FortLootTierData->NumLootPackageDrops > 0.0f)
	{
		if (FortLootTierData->NumLootPackageDrops < 1.0f)
		{
			if ((FMath::FRand() / (float)RAND_MAX) <= FortLootTierData->NumLootPackageDrops)
				NumLootPackageDrops = 1;
			else
				NumLootPackageDrops = 0;
		}
	}

	if (NumLootPackageDrops <= 0)
		return;

	TMap<int32, int32> NumLootPackageDropsPerCategory;

	if (!FortLootTier::GetNumLootPackageDropsPerCategory(FortLootTierData, NumLootPackageDrops, &NumLootPackageDropsPerCategory))
		return;

	for (const auto& [LootPackageCategory, NumDrops] : NumLootPackageDropsPerCategory)
	{
		for (int32 i = 0; i < NumDrops; i++)
			PickLootDropsFromLootPackage(OutLootToDrop, LootPackage, ForcedLootTier, LootPackageCategory, GGameState->WorldLevel);
	}
}

void FortLootPackage::PickLootDropsFromLootPackage(TArray<FFortItemEntry>* OutLootToDrop, FName LootPackage, int32 ForcedLootTier, int32 LootPackageCategory, int32 WorldLevel)
{
	UDataTable* LootPackageData = GetLootPackageData();

	if (LootPackageData == NULL)
		return;

	float TotalWeight = 0.0f;

	for (const auto& [RowName, RowValue] : LootPackageData->RowMap)
	{
		FFortLootPackageData* LootPackageRow = (FFortLootPackageData*)RowValue;

		if (LootPackageRow == NULL)
			continue;

		if (LootPackageRow->LootPackageID.ComparisonIndex != LootPackage.ComparisonIndex || LootPackageRow->LootPackageID.Number != LootPackage.Number)
			continue;

		if (LootPackageCategory != -1 && LootPackageRow->LootPackageCategory != LootPackageCategory)
			continue;

		if (WorldLevel >= 0)
		{
			if (LootPackageRow->MaxWorldLevel >= 0 && WorldLevel > LootPackageRow->MaxWorldLevel)
				continue;

			if (LootPackageRow->MinWorldLevel >= 0 && WorldLevel < LootPackageRow->MinWorldLevel)
				continue;
		}

		TotalWeight += LootPackageRow->Weight;
	}

	if (TotalWeight <= 0.0f)
		return;

	float RandomWeight = (FMath::FRand() / (float)RAND_MAX) * TotalWeight;

	for (const auto& [RowName, RowValue] : LootPackageData->RowMap)
	{
		FFortLootPackageData* LootPackageRow = (FFortLootPackageData*)RowValue;

		if (LootPackageRow == NULL)
			continue;

		if (LootPackageRow->LootPackageID.ComparisonIndex != LootPackage.ComparisonIndex || LootPackageRow->LootPackageID.Number != LootPackage.Number)
			continue;

		if (LootPackageCategory != -1 && LootPackageRow->LootPackageCategory != LootPackageCategory)
			continue;

		if (WorldLevel >= 0)
		{
			if (LootPackageRow->MaxWorldLevel >= 0 && WorldLevel > LootPackageRow->MaxWorldLevel)
				continue;

			if (LootPackageRow->MinWorldLevel >= 0 && WorldLevel < LootPackageRow->MinWorldLevel)
				continue;
		}

		if (LootPackageRow->Weight < RandomWeight)
		{
			RandomWeight -= LootPackageRow->Weight;
			continue;
		}

		if (LootPackageRow->LootPackageCall.Num() > 1)
		{
			for (int32 i = 0; i < LootPackageRow->Count; i++)
				PickLootDropsFromLootPackage(OutLootToDrop, FName(LootPackageRow->LootPackageCall), ForcedLootTier, -1, WorldLevel);

			return;
		}

		if (UFortItemDefinition* ItemDefinition = LootPackageRow->ItemDefinition.Get())
		{
			int32 Count = LootPackageRow->Count;

			if (Count)
				OutLootToDrop->Add(FFortItemEntry(ItemDefinition, Count, WorldLevel));
		}

		return;
	}
}