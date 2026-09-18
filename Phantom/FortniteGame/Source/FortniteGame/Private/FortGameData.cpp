#include "pch.h"

TArray<FItemDefinitionAndCount>* UFortGameData::GetDefaultInventory(AFortPlayerController* PlayerController, bool bForceNonFastload)
{
	TArray<FItemDefinitionAndCount> OutDefaultInventory;

	for (FItemDefinitionAndCount& InventoryItem : DefaultInventoryList)
	{
		UFortItemDefinition* ItemDefinition = InventoryItem.ItemDefinition.LoadSynchronous();

		if (ItemDefinition == NULL || InventoryItem.Count <= 0)
			continue;

		OutDefaultInventory.Add(InventoryItem);
	}

	return &OutDefaultInventory;
}