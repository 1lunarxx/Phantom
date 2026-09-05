#include "pch.h"
#include "FortniteGame/Public/Building/BuildingSMActor.h"

// unfinished

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

				UFortGameData* GameData = GetGameData();
				UFortResourceItemDefinition* ResourceItemDefinition = GameData->GetResourceItemDefinition(ResourceType);

				if (ResourceItemDefinition != NULL)
				{
					if (BuildingSMActor->MaxResourcesToSpawn < 0)
						BuildingSMActor->MaxResourcesToSpawn = BuildingSMActor->DetermineMaxResourcesToSpawn(true);

					float MaxResourcesToSpawn = BuildingSMActor->MaxResourcesToSpawn;
					int32 ResourceCount = (int32)((MaxResourcesToSpawn / BuildingSMActor->GetMaxHealth()) * ActualDamageDealt);

/*					ResourceCount = ResourceCount + BuildingSMActor->UndistributedResources;
					BuildingSMActor->UndistributedResources = ResourceCount - (float)(int32)ResourceCount;*/

					bool bDestroyed = false;

					if (!BuildingSMActor->HasHealthLeft())
						bDestroyed = true;

					if (ResourceCount > 0)
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