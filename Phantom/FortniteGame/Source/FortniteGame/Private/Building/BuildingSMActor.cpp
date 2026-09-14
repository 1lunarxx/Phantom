#include "pch.h"
#include "FortniteGame/Public/Building/BuildingSMActor.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

void BuildingSMActor::AttemptSpawnResources(ABuildingSMActor* BuildingSMActor, AFortPlayerPawn* InstigatorPawn, float ActualDamageDealt, bool bJustHitWeakspot)
{
	if (InstigatorPawn != NULL)
	{
		if (ActualDamageDealt > 0.0f && BuildingSMActor->Role == ENetRole::ROLE_Authority)
		{
			AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(InstigatorPawn->Controller);

			if (FortPlayerController != NULL)
			{
				EFortResourceType ResourceType = BuildingSMActor->ResourceType;

				UFortGameData* GameData = UFortGameData::Get();
				UFortResourceItemDefinition* ResourceItemDefinition = GameData->GetResourceItemDefinition(ResourceType);

				float ResourcesToSpawn = 0.f;
				int32 ResourceCount = 0;

				if (ResourceItemDefinition != NULL)
				{
					if (BuildingSMActor->MaxResourcesToSpawn < 0)
					{
						BuildingSMActor->MaxResourcesToSpawn = BuildingSMActor->DetermineMaxResourcesToSpawn(true);
					}

					float MaxResourcesToSpawn = (float)BuildingSMActor->MaxResourcesToSpawn;
					float MaxHealth = BuildingSMActor->GetMaxHealth();

					ResourcesToSpawn = (MaxResourcesToSpawn / MaxHealth) * ActualDamageDealt;
					ResourcesToSpawn += BuildingSMActor->UndistributedResources;

					ResourceCount = (int32)ResourcesToSpawn;

					BuildingSMActor->UndistributedResources = ResourcesToSpawn - (float)ResourceCount;
				}

				int32 Something = 1;

				bool bDestroyed = false;
				bool HasHealthLeft = BuildingSMActor->HasHealthLeft();

				if (!HasHealthLeft)
				{
					bDestroyed = true;

					if (ResourceItemDefinition == NULL || ResourcesToSpawn == 0.f)
						Something = 0;

					if (ResourceCount < Something)
						ResourceCount = Something;

					if (!BuildingSMActor->DestructionLootTierKey.IsNone())
					{
						TArray<FFortItemEntry> LootDrops;
						FortLootPackage::PickLootDrops(&LootDrops, -1, BuildingSMActor->DestructionLootTierKey);

						for (FFortItemEntry& LootDrop : LootDrops)
						{
							AFortPickup::SpawnPickup(LootDrop, BuildingSMActor->K2_GetActorLocation(), LootDrop.Count, EFortPickupSourceTypeFlag::Destruction, 0);
						}
					}
				}

				if (ResourceCount > 0)
				{
					if (BuildingSMActor->OwnerPersistentID == -1)
					{
						AFortPlayerStateZone* FortPlayerStateZone = Cast<AFortPlayerStateZone>(FortPlayerController->PlayerState);

						if (FortPlayerStateZone != NULL)
						{
							if (ResourceItemDefinition != NULL)
							{
								EFortReplicatedStat AccumulatingStatType = ResourceItemDefinition->AccumulatingStatType;

								if (AccumulatingStatType != EFortReplicatedStat::None)
									FortPlayerStateZone->ModifyReplicatedStatValues((int)AccumulatingStatType, ResourceCount, ResourceCount, BuildingSMActor->K2_GetActorLocation());
							}
						}
					}

					if (ResourceItemDefinition != NULL)
					{
						UFortWorldItem* ExistingWorldItem = FortPlayerController->WorldInventory->FindExistingItemForDefinition(ResourceItemDefinition);

						if (ExistingWorldItem != NULL)
						{
							ExistingWorldItem->ItemEntry.Count += ResourceCount;

							if (ExistingWorldItem->ItemEntry.Count >= ResourceItemDefinition->MaxStackSize)
							{
								AFortPickup::SpawnPickup(ExistingWorldItem->ItemEntry, InstigatorPawn->K2_GetActorLocation(), ExistingWorldItem->ItemEntry.Count - ResourceItemDefinition->MaxStackSize, EFortPickupSourceTypeFlag::Destruction, 0, InstigatorPawn);
								ExistingWorldItem->ItemEntry.Count = ResourceItemDefinition->MaxStackSize;
							}

							FortPlayerController->WorldInventory->UpdateItemEntry(&ExistingWorldItem->ItemEntry);
						}
						else
						{
							if (ResourceCount >= ResourceItemDefinition->MaxStackSize)
								AFortPickup::SpawnPickup(FFortItemEntry(ResourceItemDefinition, ResourceCount - ResourceItemDefinition->MaxStackSize, 0), InstigatorPawn->K2_GetActorLocation(), ResourceCount - ResourceItemDefinition->MaxStackSize, EFortPickupSourceTypeFlag::Destruction, 0, InstigatorPawn);

							FortPlayerController->WorldInventory->AddItem(ResourceItemDefinition, ResourceCount);
						}

						FortPlayerController->ClientReportDamagedResourceBuilding(BuildingSMActor, ResourceType, ResourceCount, bDestroyed, bJustHitWeakspot);
					}
				}
			}
		}
	}
}

void BuildingSMActor::Setup()
{
	Utils::Virtual(ABuildingSMActor::GetDefaultObj(), 0xA40 / 8, AttemptSpawnResources);
}