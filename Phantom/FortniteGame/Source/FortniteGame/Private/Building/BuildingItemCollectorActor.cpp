#include "pch.h"
#include "FortniteGame/Public/Building/BuildingItemCollectorActor.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"
#include "Core/Public/Math/UnrealMathUtility.h"
#include <FortniteGame/Public/Items/FortLootLevel.h>

void ABuildingItemCollectorActor::FillOutRandomLoot()
{
    StartingGoalLevel = FMath::RandRange(2, 5); // scuffed but uh

    for (FCollectorUnitInfo& ItemCollection : ItemCollections)
    {
        if (ItemCollection.bUseDefinedOutputItem)
            continue;

        FName LootTierGroupName = ItemCollection.OverrideOutputItemLootTierGroupName;

        if (LootTierGroupName.IsNone())
            LootTierGroupName = DefaultItemLootTierGroupName;

        UFortLootPackage::PickLootDrops(&ItemCollection.OutputItemEntry, StartingGoalLevel, LootTierGroupName);
    }
}

void BuildingItemCollectorActor::FinishItemSelectionSetup(ABuildingItemCollectorActor* BuildingItemCollectorActor)
{
    Originals::FinishItemSelectionSetup(BuildingItemCollectorActor);

    if (BuildingItemCollectorActor->OverrideOutputItems.Num() <= 0)
        BuildingItemCollectorActor->FillOutRandomLoot();
}

void BuildingItemCollectorActor::OnReachedDepositGoal(ABuildingItemCollectorActor* BuildingItemCollectorActor, AFortPlayerController* CurrentControllingPlayer, UFortWorldItemDefinition* CurrentActiveInputItem)
{
    printf(__FUNCTION__);
}

void BuildingItemCollectorActor::Setup()
{
    Utils::Hook(InSDKUtils::GetImageBase() + 0xD99F30, FinishItemSelectionSetup, (void**)&Originals::FinishItemSelectionSetup);
    Utils::Hook(InSDKUtils::GetImageBase() + 0x14AA610, OnReachedDepositGoal);
}