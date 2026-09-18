#include "pch.h"
#include "FortniteGame/Public/Items/FortInventory.h"
#include "FortniteGame/Public/Items/FortLootLevel.h"

UFortWorldItem* AFortInventory::AddItem(UFortItemDefinition* ItemDefinition, int32 Count)
{
	UFortWorldItemDefinition* WorldItemDefinition = Cast<UFortWorldItemDefinition>(ItemDefinition);

	if (WorldItemDefinition == NULL)
		return NULL;
			
	FFortItemEntry ItemToAddEntry = FFortItemEntry(WorldItemDefinition, Count, UFortLootLevel::GetItemLevel(&WorldItemDefinition->LootLevelData, GetWorld()->GetGameState()->WorldLevel));

	if (ItemToAddEntry.Count <= 0)
		return NULL;

	return AddItem(&ItemToAddEntry);
}

UFortWorldItem* AFortInventory::AddItem(FFortItemEntry* ItemEntry)
{
	UFortWorldItem* WorldItem = UFortWorldItem::New(this, *ItemEntry);

	if (WorldItem == NULL)
		return NULL;

	InitializeExistingItem(WorldItem);

	if (AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(GetOwner()))
	{
		WorldItem->SetOwningControllerForTemporaryItem(FortPlayerController);

		if (IFortInventoryOwnerInterface* FortInventoryOwnerInterface = FortPlayerController->GetInterfaceAddress<IFortInventoryOwnerInterface>())
		{
			WorldItem->OnItemInstanceAdded(FortInventoryOwnerInterface);
		}

		if (CurrentSubGame == ESubGame::Campaign)
			FortPlayerController->TryAddToQuickBar(WorldItem); // should get called auto idfk
	}

	return WorldItem;
}

void AFortInventory::AddItemStack(UFortItemDefinition* ItemDefinition, int32 Count)
{
	if (UFortWorldItem* ExistingItem = FindExistingItemForDefinition(ItemDefinition))
	{
		ExistingItem->ItemEntry.SetCount(ExistingItem->ItemEntry.Count + Count);
	}
	else
	{
		AddItem(ItemDefinition, Count);
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
		if (AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(GetOwner()))
		{
			if (IFortInventoryOwnerInterface* FortInventoryOwnerInterface = FortPlayerController->GetInterfaceAddress<IFortInventoryOwnerInterface>())
			{
				WorldItem->OnItemInstanceRemoved(FortInventoryOwnerInterface, ItemEntry->Count);
			}
		}

		WorldItem->RemoveFromInventory();

		OnRemoveItemStack(WorldItem, &ItemGuid);
	}
}

void AFortInventory::RemoveItem(FGuid& ItemGuid, int32 Count)
{
	FFortItemEntry* ItemEntry = GetReplicatedItemEntry(&ItemGuid);

	if (ItemEntry == NULL)
		return;

	if (Count <= 0 || Count >= ItemEntry->Count)
	{
		RemoveItem(ItemGuid);
	}
	else
	{
		ItemEntry->SetCount(ItemEntry->Count - Count);
	}
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

			RemovedItemEntry.StateValues.Remove(i);

			break;
		}

		UpdateItemInstances();
		HandleInventoryItemRemoved();
	}
}

FFortItemEntry* AFortInventory::GetReplicatedItemEntry(FGuid* ItemGuid)
{
	if (Inventory.ReplicatedEntries.Num() <= 0)
		return NULL;

	for (FFortItemEntry& ItemEntry : Inventory.ReplicatedEntries)
	{
		if (ItemEntry.ItemGuid == *ItemGuid)
			return &ItemEntry;
	}

	return NULL;
}