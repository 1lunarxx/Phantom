#include "pch.h"
#include "FortniteGame/Public/Building/BuildingSMActor.h"
#include "Core/Public/Math/UnrealMathUtility.h"

void BuildingSMActor::AttemptSpawnResources(ABuildingSMActor* BuildingSMActor, AFortPlayerPawn* InstigatorPawn, float ActualDamageDealt, bool bJustHitWeakspot)
{
	AFortPlayerController* PlayerController = Cast<AFortPlayerController>(InstigatorPawn->Controller);

	if (PlayerController == NULL)
		return;

	FCurveTableRowHandle& BuildingResourceAmountOverride = BuildingSMActor->BuildingResourceAmountOverride;

	if (!BuildingResourceAmountOverride.RowName.ComparisonIndex)
		return;

	float Result = 0.f;

	UFortKismetLibrary::EvaluateCurveTableRow(BuildingResourceAmountOverride, 0.f, &Result, FString());

	int32 ResourceCount = FMath::RoundToInt(Result / (BuildingSMActor->GetMaxHealth() / ActualDamageDealt));

	if (ResourceCount <= 0)
		return;

	UFortResourceItemDefinition* ResourceItemDefinition = UFortKismetLibrary::K2_GetResourceItemDefinition(BuildingSMActor->ResourceType);

	if (ResourceItemDefinition == NULL)
		return;

	int32 MaxResourcesToSpawn = ResourceItemDefinition->MaxStackSize;

	if (MaxResourcesToSpawn <= 0)
		return;

	UFortWorldItem* ExistingWorldItem = PlayerController->WorldInventory->FindExistingItemForDefinition(ResourceItemDefinition);

	if (ExistingWorldItem != NULL)
	{
		ExistingWorldItem->ItemEntry.Count += ResourceCount;

		if (ExistingWorldItem->ItemEntry.Count >= MaxResourcesToSpawn)
		{
			AFortPickup::SpawnPickup(ExistingWorldItem->ItemEntry, InstigatorPawn->K2_GetActorLocation(), ExistingWorldItem->ItemEntry.Count - MaxResourcesToSpawn, EFortPickupSourceTypeFlag::Player, 0, InstigatorPawn);
			ExistingWorldItem->ItemEntry.Count = MaxResourcesToSpawn;
		}

		PlayerController->WorldInventory->UpdateItemEntry(&ExistingWorldItem->ItemEntry);
	}
	else
	{
		if (ResourceCount >= MaxResourcesToSpawn)
			AFortPickup::SpawnPickup(FFortItemEntry(ResourceItemDefinition, ResourceCount - MaxResourcesToSpawn, 0), InstigatorPawn->K2_GetActorLocation(), ResourceCount - MaxResourcesToSpawn, EFortPickupSourceTypeFlag::Player, 0, InstigatorPawn);

		PlayerController->WorldInventory->AddItem(ResourceItemDefinition, ResourceCount);
	}

	PlayerController->ClientReportDamagedResourceBuilding(BuildingSMActor, BuildingSMActor->ResourceType, ResourceCount, BuildingSMActor->bDestroyed, bJustHitWeakspot);
}

void BuildingSMActor::Setup()
{
	Utils::Virtual(ABuildingSMActor::GetDefaultObj(), 0xA40 / 8, AttemptSpawnResources);
}