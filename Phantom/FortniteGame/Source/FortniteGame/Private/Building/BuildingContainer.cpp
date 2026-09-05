#include "pch.h"
#include "FortniteGame/Public/Building/BuildingContainer.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

void BuildingContainer::PostUpdate(ABuildingContainer* BuildingContainer, EFortBuildingPersistentState PersistentState, const void* ReservedRandomValues)
{
	Originals::PostUpdate(BuildingContainer, PersistentState, ReservedRandomValues);

	if (BuildingContainer->bStartAlreadySearched_Athena)
		BuildingContainer->SpawnLoot();
}

bool BuildingContainer::SpawnLoot(ABuildingContainer* BuildingContainer, AFortPlayerPawn* PlayerPawn, const EFortPickupSourceTypeFlag InSourceTypeFlag, const uint8 InSpawnSource)
{
	if (PlayerPawn != NULL)
	{
		FVector BounceNormal = PlayerPawn->K2_GetActorLocation() - BuildingContainer->K2_GetActorLocation();
		BounceNormal.Z = 0.0f;

		if (!BounceNormal.IsZero())
			BounceNormal.Normalize();

		BuildingContainer->SearchBounceData.BounceNormal = BounceNormal;
	}

	BuildingContainer->SearchBounceData.SearchAnimationCount++;
	BuildingContainer->BounceContainer();

	BuildingContainer->bAlreadySearched = true;
	BuildingContainer->OnRep_bAlreadySearched();

	if (BuildingContainer->SearchLootTierGroup == FName(L"Loot_Treasure"))
		BuildingContainer->SearchLootTierGroup = FName(L"Loot_AthenaTreasure");

	else if (BuildingContainer->SearchLootTierGroup == FName(L"Loot_Ammo"))
		BuildingContainer->SearchLootTierGroup = FName(L"Loot_AthenaAmmoLarge");

	TArray<FFortItemEntry> OutLootDrops;
	FortLootPackage::PickLootDrops(&OutLootDrops, GGameState->WorldLevel, BuildingContainer->SearchLootTierGroup);

	FVector LocationToSpawn = BuildingContainer->K2_GetActorLocation() + (BuildingContainer->GetActorForwardVector() * BuildingContainer->LootSpawnLocation_Athena.X) + (BuildingContainer->GetActorRightVector() * BuildingContainer->LootSpawnLocation_Athena.Y) + (BuildingContainer->GetActorUpVector() * BuildingContainer->LootSpawnLocation_Athena.Z);

	for (FFortItemEntry& LootDrop : OutLootDrops)
	{
		AFortPickup::SpawnPickup(LootDrop, LocationToSpawn, LootDrop.Count, InSourceTypeFlag, InSpawnSource, false, true, NULL, BuildingContainer);
	}

    return true;
}

void BuildingContainer::Setup()
{
    Utils::Hook(InSDKUtils::GetImageBase() + 0xD867F0, SpawnLoot);
	Utils::Hook(InSDKUtils::GetImageBase() + 0xD7FB30, PostUpdate, (void**)&Originals::PostUpdate);
}