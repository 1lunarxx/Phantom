#include "pch.h"
#include "FortniteGame/Public/Items/FortInventory.h"

void AFortInventory::AddItem(UFortItemDefinition* ItemDefinition, int32 Count)
{
	if (Count <= 0)
		return;

	UFortWorldItem* WorldItem = UFortWorldItem::New(this, FFortItemEntry(ItemDefinition, Count, 0));

	if (WorldItem == NULL)
		return;

	if (AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(GetOwner()))
	{
		WorldItem->SetOwningControllerForTemporaryItem(FortPlayerController);
	}

	InitializeExistingItem(WorldItem);
}

// def a better way for all these functions, just dont care to be proper.

void AFortInventory::RemoveItem(FGuid& ItemGuid)
{
	FFortItemEntry* ItemEntry = GetReplicatedItemEntry(&ItemGuid);

	if (ItemEntry == NULL)
		return;

	UFortWorldItem* WorldItem = FindExistingItemForDefinition(ItemEntry->ItemDefinition);

	if (WorldItem == NULL)
		return;

	Inventory.ReplicatedEntries.Remove(ItemEntry);
	Inventory.ItemInstances.Remove(&WorldItem);

	Inventory.MarkArrayDirty();

	bRequiresLocalUpdate = true;
	HandleInventoryLocalUpdate();
}

void AFortInventory::UpdateItemEntry(FFortItemEntry* NewItemEntry)
{
	FFortItemEntry* ItemEntry = GetReplicatedItemEntry(&NewItemEntry->ItemGuid);

	if (ItemEntry == NULL)
		return;

	*ItemEntry = *NewItemEntry;

	Inventory.MarkItemDirty(*ItemEntry);

	bRequiresLocalUpdate = true;
	HandleInventoryLocalUpdate();
}

FFortItemEntry* AFortInventory::GetReplicatedItemEntry(FGuid* ItemGuid)
{
	if (!(ItemGuid->D | ItemGuid->A | ItemGuid->C | ItemGuid->B) || Inventory.ReplicatedEntries.Num() <= 0)
		return NULL;

	for (FFortItemEntry& ItemEntry : Inventory.ReplicatedEntries)
	{
		if (ItemEntry.ItemGuid == *ItemGuid)
			return &ItemEntry;
	}

	return NULL;
}