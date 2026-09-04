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

// def a better way i just dont care enough

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

UFortWorldItem* AFortInventory::FindExistingItemForDefinition(UFortItemDefinition* ItemDefinition, bool bInStorageVault)
{
	for (UFortWorldItem* ItemInstance : Inventory.ItemInstances)
	{
		if (ItemInstance->ItemEntry.ItemDefinition == ItemDefinition)
			return ItemInstance;
	}

	return NULL;
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