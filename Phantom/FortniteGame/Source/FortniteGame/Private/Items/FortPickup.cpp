#include "pch.h"
#include "Utils.h"

/*
	PlayerElimination = 1,
	Chest = 2,
	SupplyDrop = 3,
	AmmoBox = 4,
*/

AFortPickup* AFortPickup::SpawnPickup(FFortItemEntry ItemEntry, FVector InLocation, int32 Count, EFortPickupSourceTypeFlag PickupSourceTypeFlag, bool bRandomRotation, bool bToss, AFortPlayerPawn* PlayerPawn, ABuildingContainer* Container)
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
	CreationData->SpawnSource = Container != NULL ? Container->SearchLootTierGroup.ToString().contains("Treasure") ? 2 : Container->SearchLootTierGroup.ToString().contains("Ammo") ? 4 : 0 : 0;CreationData->bRandomRotation = bRandomRotation;
	CreationData->bPickupOnlyRelevantToOwner = bToss;

	AFortPickup* Pickup = AFortPickup::CreateFromData(CreationData);

	if (Pickup != NULL)
	{
		Pickup->PawnWhoDroppedPickup = PlayerPawn;

		Pickup->SetPickupItems(&Pickup->PrimaryPickupItemEntry, false);
		Pickup->TossPickup(InLocation, PlayerPawn, -1, bToss, PickupSourceTypeFlag);
	}

	return Pickup;
}