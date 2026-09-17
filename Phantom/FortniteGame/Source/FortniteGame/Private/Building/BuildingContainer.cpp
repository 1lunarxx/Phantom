#include "pch.h"
#include "FortniteGame/Public/Building/BuildingContainer.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

#include "Core/Public/Math/UnrealMathUtility.h"

void BuildingContainer::PostUpdate(ABuildingContainer* BuildingContainer, EFortBuildingPersistentState PersistentState, const void* ReservedRandomValues)
{
	Originals::PostUpdate(BuildingContainer, PersistentState, ReservedRandomValues);

	if (BuildingContainer->bStartAlreadySearched_Athena)
		BuildingContainer->SpawnLoot();
}

bool BuildingContainer::SpawnLoot(ABuildingContainer* BuildingContainer, AFortPlayerPawn* PlayerPawn, const EFortPickupSourceTypeFlag InSourceTypeFlag, const uint8 InSpawnSource)
{
	FVector LootDropLocation = BuildingContainer->K2_GetActorLocation();

	if (CurrentSubGame == ESubGame::Athena)
	{
		LootDropLocation = LootDropLocation + (BuildingContainer->GetActorForwardVector() * BuildingContainer->LootSpawnLocation_Athena.X) + (BuildingContainer->GetActorRightVector() * BuildingContainer->LootSpawnLocation_Athena.Y) + (BuildingContainer->GetActorUpVector() * BuildingContainer->LootSpawnLocation_Athena.Z);

		if (BuildingContainer->SearchLootTierGroup == FName(L"Loot_Treasure"))
			BuildingContainer->SearchLootTierGroup = FName(L"Loot_AthenaTreasure");

		else if (BuildingContainer->SearchLootTierGroup == FName(L"Loot_Ammo"))
			BuildingContainer->SearchLootTierGroup = FName(L"Loot_AthenaAmmoLarge");
	}

	TArray<FFortItemEntry> OutLootDrops;
	UFortLootPackage::PickLootDrops(&OutLootDrops, BuildingContainer->GetLootTier(), BuildingContainer->SearchLootTierGroup);

	for (FFortItemEntry& LootDrop : OutLootDrops)
	{
		if (UFortWeaponRangedItemDefinition* WeaponRangedItemDefinition = Cast<UFortWeaponRangedItemDefinition>(LootDrop.ItemDefinition))
		{
			FFortRangedWeaponStats OutRow;
			UFortKismetLibrary::GetRangedWeaponStatsRow(WeaponRangedItemDefinition->WeaponStatHandle, &OutRow);

			LootDrop.LoadedAmmo = OutRow.ClipSize;
		}

		AFortPickup* FortPickup = AFortPickup::SpawnPickup(LootDrop, LootDropLocation, LootDrop.Count, InSourceTypeFlag, InSpawnSource, false, true, NULL, BuildingContainer);

		if (FortPickup != NULL && CurrentSubGame == ESubGame::Campaign)
			FortPickup->SetPickupTarget(PlayerPawn, FortPickup->GetFlyTime(), FMath::VRandCone(FVector(0,0,1), 0.0f));
	}

	BuildingContainer->SearchBounceData.SearchAnimationCount++;
	BuildingContainer->BounceContainer();

	BuildingContainer->bAlreadySearched = true;
	BuildingContainer->OnRep_bAlreadySearched();

	if (CurrentSubGame == ESubGame::Campaign && BuildingContainer->bDestroyContainerOnSearch)
		BuildingContainer->K2_DestroyActor();

	return true;
}

void BuildingContainer::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0xD867F0, SpawnLoot);
	Utils::Hook(InSDKUtils::GetImageBase() + 0xD7FB30, PostUpdate, (void**)&Originals::PostUpdate);
}