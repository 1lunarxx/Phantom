#include "pch.h"
#include "FortniteGame/Public/Items/FortPickup.h"
#include "FortniteGame/Public/Items/FortLootLevel.h"

void FortPickup::GivePickupTo(AFortPickup* FortPickup, IFortInventoryOwnerInterface* InventoryOwner, bool DestoryAfterPickup)
{
	Originals::GivePickupTo(FortPickup, InventoryOwner, DestoryAfterPickup);

	AFortPlayerController* FortPlayerController = InventoryOwner->GetOwner<AFortPlayerController>();

	if (FortPlayerController == NULL)
		return;

	FFortItemEntry* PickupItemEntry = &FortPickup->PrimaryPickupItemEntry;

	if (PickupItemEntry == NULL)
		return;

	if (AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory())
	{
		int32 OverflowFromAddingItem = WorldInventory->GetOverflowFromAddingItem(PickupItemEntry);

		if (OverflowFromAddingItem <= 0)
		{
			if (UFortItemDefinition* FortItemDefinition = PickupItemEntry->ItemDefinition)
			{
				UFortWorldItem* ExistingWorldItem = WorldInventory->FindExistingItemForDefinition(FortItemDefinition);

				if (ExistingWorldItem != NULL && FortItemDefinition->IsStackable())
				{
					ExistingWorldItem->ItemEntry.SetCount(ExistingWorldItem->ItemEntry.Count + PickupItemEntry->Count);
				}
				else
				{
					WorldInventory->AddItem(PickupItemEntry);
				}
			}
		}
		else
		{
			int32 Count = PickupItemEntry->Count - OverflowFromAddingItem;

			if (Count > 0)
			{
				if (UFortItemDefinition* FortItemDefinition = PickupItemEntry->ItemDefinition)
				{
					UFortWorldItem* ExistingWorldItem = WorldInventory->FindExistingItemForDefinition(FortItemDefinition);

					if (ExistingWorldItem != NULL && FortItemDefinition->IsStackable())
					{
						ExistingWorldItem->ItemEntry.SetCount(ExistingWorldItem->ItemEntry.Count + Count);
					}
					else
					{
						WorldInventory->AddItem(PickupItemEntry);
					}

					AFortPickup* Pickup = AFortPickup::CreateFromData(FortPickupCreationData(GWorld, PickupItemEntry, FortPlayerController->GetViewTarget()->K2_GetActorLocation(), FRotator(), FortPlayerController, NULL, NULL, EFortPickupSourceTypeFlag::Player, 0, true, false));

					if (Pickup != NULL)
						Pickup->PawnWhoDroppedPickup = FortPlayerController->GetPlayerPawn();
				}
			}
			else
			{
				FFortItemEntry* CurrentWeaponEntry = WorldInventory->GetReplicatedItemEntry(&FortPlayerController->GetPlayerPawn()->CurrentWeapon->ItemEntryGuid);

				if (CurrentWeaponEntry != NULL)
				{
					WorldInventory->RemoveItem(CurrentWeaponEntry->ItemGuid);

					AFortPickup* Pickup = AFortPickup::CreateFromData(FortPickupCreationData(GWorld, CurrentWeaponEntry, FortPlayerController->GetViewTarget()->K2_GetActorLocation(), FRotator(), FortPlayerController, NULL, NULL, EFortPickupSourceTypeFlag::Player, 0, true, false));

					if (Pickup != NULL)
						Pickup->PawnWhoDroppedPickup = FortPlayerController->GetPlayerPawn();

					WorldInventory->AddItem(PickupItemEntry);
				}
			}
		}
	}
}

void FortPickup::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x109A490, GivePickupTo, (void**)&Originals::GivePickupTo);
}