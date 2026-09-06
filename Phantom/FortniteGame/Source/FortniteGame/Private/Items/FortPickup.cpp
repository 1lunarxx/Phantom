#include "pch.h"
#include "FortniteGame/Public/Items/FortPickup.h"

AFortPickup* AFortPickup::SpawnPickup(FFortItemEntry ItemEntry, FVector InLocation, int32 Count, EFortPickupSourceTypeFlag PickupSourceTypeFlag, uint8 SpawnSource, bool bRandomRotation, bool bToss, AFortPlayerPawn* PlayerPawn, ABuildingContainer* Container)
{
	if (Count)
		ItemEntry.Count = Count;

	FortPickupCreationData* CreationData = new FortPickupCreationData();

	CreationData->World = UWorld::GetWorld();
	CreationData->PickupDataItemEntry = &ItemEntry;
	CreationData->Position = &InLocation;
	CreationData->OptionalPCOwner = NULL;
	CreationData->OverrideClass = NULL;
	CreationData->OwnerContainer = Container;
	CreationData->SourceTypeFlags = PickupSourceTypeFlag;
	CreationData->SpawnSource = SpawnSource;
	CreationData->bPickupOnlyRelevantToOwner = bToss;

	AFortPickup* Pickup = AFortPickup::CreateFromData(CreationData);

	if (Pickup != NULL)
	{
		Pickup->PawnWhoDroppedPickup = PlayerPawn;
		Pickup->TossPickup(InLocation, Pickup->PawnWhoDroppedPickup, -1, bToss, PickupSourceTypeFlag);
	}

	return Pickup;
}

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
					ExistingWorldItem->ItemEntry.Count += PickupItemEntry->Count;
					WorldInventory->UpdateItemEntry(&ExistingWorldItem->ItemEntry);
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
						ExistingWorldItem->ItemEntry.Count += Count;
						WorldInventory->UpdateItemEntry(&ExistingWorldItem->ItemEntry);
					}
					else
					{
						WorldInventory->AddItem(PickupItemEntry);
					}

					AFortPickup::SpawnPickup(*PickupItemEntry, FortPlayerController->GetViewTarget()->K2_GetActorLocation(), OverflowFromAddingItem, EFortPickupSourceTypeFlag::Player, -1, true, true, FortPlayerController->GetPlayerPawn());
				}
			}
			else
			{
				FFortItemEntry* CurrentWeaponEntry = WorldInventory->GetReplicatedItemEntry(&FortPlayerController->GetPlayerPawn()->CurrentWeapon->ItemEntryGuid);

				if (CurrentWeaponEntry != NULL)
				{
					AFortPickup::SpawnPickup(*CurrentWeaponEntry, FortPlayerController->GetViewTarget()->K2_GetActorLocation(), CurrentWeaponEntry->Count, EFortPickupSourceTypeFlag::Player, -1, true, true, FortPlayerController->GetPlayerPawn());

					WorldInventory->RemoveItem(CurrentWeaponEntry->ItemGuid);
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