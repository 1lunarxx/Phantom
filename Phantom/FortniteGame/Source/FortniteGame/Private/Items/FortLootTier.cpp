#include "pch.h"
#include "FortniteGame/Public/Items/FortLootTier.h"

// aids

bool FortLootTier::GetNumLootPackageDropsPerCategory(FFortLootTierData* LootTierData, int32 NumLootPackageDrops, TMap<int32, int32>* OutNumLootPackageDropsPerCategory)
{
	if (LootTierData == NULL || OutNumLootPackageDropsPerCategory == NULL)
		return false;

	int32 v64 = 0;
	TArray<int32> v22;

	for (int32 v9 = 0; v9 < LootTierData->LootPackageCategoryWeightArray.Num(); v9++)
		v22.Add(0);

	for (int32 v9 = 0; v9 < LootTierData->LootPackageCategoryMinArray.Num(); v9++)
	{
		float v12 = LootTierData->LootPackageCategoryMinArray[v9];

		if (v12 > 0.0f)
		{
			int32 v63 = (int32)v12;

			if (v9 < v22.Num())
				v22[v9] = v63;

			v64 += v63;
		}
	}

	float v63 = 0.0f;
	TArray<int32> v70;

	for (int32 v15 = 0; v15 < LootTierData->LootPackageCategoryWeightArray.Num(); v15++)
	{
		float v24 = LootTierData->LootPackageCategoryWeightArray[v15];

		if (v24 <= 0.0f)
			continue;

		int32 v18 = -1;

		if (v15 < LootTierData->LootPackageCategoryMaxArray.Num())
			v18 = (int32)LootTierData->LootPackageCategoryMaxArray[v15];

		int32 v22Value = 0;

		if (v15 < v22.Num())
			v22Value = v22[v15];

		if (v18 < 0 || v18 > v22Value)
		{
			v63 += v24;
			v70.Add(v15);
		}
	}

	while (v64 < NumLootPackageDrops && v63 > 0.0f)
	{
		float v29 = UKismetMathLibrary::RandomFloat() * v63;
		int32 v35 = -1;

		for (int32 v9 = 0; v9 < v70.Num(); v9++)
		{
			int32 v15 = v70[v9];
			float v33 = LootTierData->LootPackageCategoryWeightArray[v15];

			if (v29 <= v33)
			{
				v35 = v15;
				break;
			}

			v29 -= v33;
		}

		if (v35 < 0)
			break;

		if (v35 >= v22.Num())
			break;

		++v22[v35];
		++v64;

		int32 v37 = -1;

		if (v35 < LootTierData->LootPackageCategoryMaxArray.Num())
			v37 = (int32)LootTierData->LootPackageCategoryMaxArray[v35];

		if (v37 >= 0 && v22[v35] >= v37)
		{
			v63 -= LootTierData->LootPackageCategoryWeightArray[v35];

			for (int32 v9 = 0; v9 < v70.Num(); v9++)
			{
				if (v70[v9] == v35)
				{
					v70.Remove(v9);
					break;
				}
			}
		}
	}

	for (int32 v9 = 0; v9 < v22.Num(); v9++)
	{
		if (v22[v9] > 0)
			OutNumLootPackageDropsPerCategory->Add(v9, v22[v9]);
	}

	return true;
}