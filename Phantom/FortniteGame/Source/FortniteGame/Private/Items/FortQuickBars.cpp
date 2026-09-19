#include "pch.h"

// stripped because fortnite loves cock 

void AFortQuickBars::InitializeDefaultInventory(AFortPlayerController* FortPC)
{
	UFortGameData* GameData = UFortGameData::Get();
	TArray<FItemDefinitionAndCount>* DefaultInventory = GameData->GetDefaultInventory(FortPC, false);

	if (DefaultInventory->Num() <= 0)
		return;

	AFortInventory* WorldInventory = FortPC->GetWorldInventory();

	for (FItemDefinitionAndCount& InventoryItem : *DefaultInventory)
	{
		UFortItemDefinition* ItemDefinition = InventoryItem.ItemDefinition.LoadSynchronous();

		if (ItemDefinition == NULL)
			continue;

		UFortWorldItem* AddedItem = WorldInventory->AddItem(ItemDefinition, InventoryItem.Count);

		if (AddedItem == NULL)
			continue;
	}
}