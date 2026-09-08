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

	if (IFortInventoryOwnerInterface* FortInventoryOwnerInterface = GetInterfaceAddress<IFortInventoryOwnerInterface>())
	{
		WorldItem->OnItemInstanceAdded(FortInventoryOwnerInterface);
	}

	InitializeExistingItem(WorldItem);
}

void AFortInventory::AddItem(FFortItemEntry* ItemEntry)
{
	UFortWorldItem* WorldItem = UFortWorldItem::New(this, *ItemEntry);

	if (WorldItem == NULL)
		return;

	if (AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(GetOwner()))
	{
		WorldItem->SetOwningControllerForTemporaryItem(FortPlayerController);
	}

	if (IFortInventoryOwnerInterface* FortInventoryOwnerInterface = GetInterfaceAddress<IFortInventoryOwnerInterface>())
	{
		WorldItem->OnItemInstanceAdded(FortInventoryOwnerInterface);
	}

	InitializeExistingItem(WorldItem);
}

void AFortInventory::OnRemoveItemStack(UFortWorldItem* ItemStackToRemove, const FGuid* ItemGuid)
{
	if (ItemStackToRemove != NULL)
	{
		UFortItemDefinition* FortItemDefinition = ItemStackToRemove->ItemEntry.ItemDefinition;

		if (FortItemDefinition != NULL)
		{
			if (UFortWorldItemDefinition* FortWorldItemDefinition = Cast<UFortWorldItemDefinition>(FortItemDefinition))
			{
				if (!ItemStackToRemove->IsInventoryOverflowItem())
				{
					for (UFortWorldItem* WorldItem : Inventory.ItemInstances)
					{
						if (WorldItem == NULL)
							continue;

						UFortWorldItemDefinition* WorldItemDefinition = Cast<UFortWorldItemDefinition>(WorldItem->ItemEntry.ItemDefinition);

						if (WorldItemDefinition == NULL)
							continue;

						if (WorldItem->IsInventoryOverflowItem())
						{
							WorldItem->SetInInventoryOverflow(false);
							break;
						}
					}
				}

				ItemStackToRemove->SetInInventoryOverflow(false);
			}
		}

		FFortItemEntry RemovedItemEntry = FFortItemEntry{};

		if (Inventory.ReplicatedEntries.Num() > 0)
		{
			for (int32 i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
			{
				FFortItemEntry& ItemEntry = Inventory.ReplicatedEntries[i];

				if (ItemEntry.ItemGuid == *ItemGuid)
				{
					RemovedItemEntry = ItemEntry;
					Inventory.ReplicatedEntries.Remove(i);
					break;
				}
			}
		}

		for (int32 i = 0; i < RemovedItemEntry.StateValues.Num(); i++)
		{
			FFortItemEntryStateValue& StateValue = RemovedItemEntry.StateValues[i];

			if (StateValue.StateType != EFortItemEntryState::EFortItemEntryState_MAX)
				continue;

			RemovedItemEntry.StateValues.Remove(i); // they dont do this but why not

			break;
		}

		UpdateItemInstances();
		HandleInventoryItemRemoved();
	}
}

void AFortInventory::RemoveItem(FGuid& ItemGuid)
{
	FFortItemEntry* ItemEntry = GetReplicatedItemEntry(&ItemGuid);

	if (ItemEntry == NULL)
		return;

	UFortWorldItem* WorldItem = FindExistingItemForDefinition(ItemEntry->ItemDefinition);

	if (WorldItem != NULL)
	{
		if (IFortInventoryOwnerInterface* FortInventoryOwnerInterface = GetInterfaceAddress<IFortInventoryOwnerInterface>())
		{
			WorldItem->OnItemInstanceRemoved(FortInventoryOwnerInterface, ItemEntry->Count);
		}

		WorldItem->RemoveFromInventory();

		OnRemoveItemStack(WorldItem, &ItemGuid);
	}
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