#include "pch.h"
#include "FortniteGame/Public/Building/BuildingSMActor.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

#include "Core/Public/Math/UnrealMathUtility.h"

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

					if (!BuildingSMActor->DestructionLootTierGroup.IsNone())
					{
						int32 WorldLevel = -1;

						if (AFortGameState* GameState = FortPlayerController->GetWorld()->GetGameState())
						{
							WorldLevel = GameState->WorldLevel;
						}

						TArray<FFortItemEntry> OutLootDrops;
						UFortLootPackage::PickLootDrops(&OutLootDrops, WorldLevel, BuildingSMActor->DestructionLootTierGroup);

						for (FFortItemEntry& LootDrop : OutLootDrops)
						{
							AFortPickup* FortPickup = AFortPickup::CreateFromData(FortPickupCreationData(GWorld, &LootDrop, BuildingSMActor->K2_GetActorLocation(), FRotator(), NULL, NULL, BuildingSMActor, EFortPickupSourceTypeFlag::Destruction, 0, true, false));

							if (FortPickup != NULL)
								FortPickup->SetPickupTarget(InstigatorPawn, FortPickup->GetFlyTime(), FMath::VRandCone(FVector(0, 0, 1), 0.0f));
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
						if (UFortWorldItem* ExistingItem = FortPlayerController->WorldInventory->FindExistingItemForDefinition(ResourceItemDefinition))
						{
							FFortItemEntry* ItemEntry = ExistingItem->GetItemEntry();

							ExistingItem->ItemEntry.SetCount(ItemEntry->Count + ResourceCount);

							if (ItemEntry->Count >= ResourceItemDefinition->MaxStackSize)
							{
								ItemEntry->Count = ItemEntry->Count - ResourceItemDefinition->MaxStackSize;

								AFortPickup* FortPickup = AFortPickup::CreateFromData(FortPickupCreationData(GWorld, ItemEntry, BuildingSMActor->K2_GetActorLocation(), FRotator(), NULL, NULL, BuildingSMActor, EFortPickupSourceTypeFlag::Destruction, 0, true, false));

								if (FortPickup != NULL)
									ItemEntry->SetCount(ResourceItemDefinition->MaxStackSize);
							}
						}
						else
						{
							if (ResourceCount >= ResourceItemDefinition->MaxStackSize)
							{
								FFortItemEntry ItemEntry = FFortItemEntry(ResourceItemDefinition, ResourceCount - ResourceItemDefinition->MaxStackSize, 0);
								AFortPickup::CreateFromData(FortPickupCreationData(GWorld, &ItemEntry, BuildingSMActor->K2_GetActorLocation(), FRotator(), NULL, NULL, BuildingSMActor, EFortPickupSourceTypeFlag::Destruction, 0, true, false));
							}

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