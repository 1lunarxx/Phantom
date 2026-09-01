#include "pch.h"
#include "FortniteGame/Public/Items/FortInventory.h"

void AFortInventory::AddItem(UFortItemDefinition* ItemDefinition, int32 Count)
{
	UFortWorldItem* WorldItem = UFortWorldItem::New(this, FFortItemEntry(ItemDefinition, Count, 0));

	if (WorldItem == NULL)
		return;

	if (AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(GetOwner()))
	{
		WorldItem->SetOwningControllerForTemporaryItem(FortPlayerController);
	}

	InitializeExistingItem(WorldItem);
}

UFortWorldItem* AFortInventory::FindExistingItemForDefinition(UFortItemDefinition* ItemDefinition, bool bInStorageVault)
{
	for (UFortWorldItem* ItemInstance : Inventory.ItemInstances)
	{
		if (ItemInstance->ItemEntry.ItemDefinition == ItemDefinition)
			return ItemInstance;
	}

	return NULL;
}