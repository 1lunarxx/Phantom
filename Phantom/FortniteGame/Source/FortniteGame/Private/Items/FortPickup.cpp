#include "pch.h"
#include "Utils.h"

/*
	PlayerElimination = 1,
	Chest = 2,
	SupplyDrop = 3,
	AmmoBox = 4,
*/

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