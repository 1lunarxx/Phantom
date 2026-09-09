#include "pch.h"
#include "FortniteGame/Public/FortKismetLibrary.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

AFortAIGoalManager* FortKismetLibrary::GetAIGoalManager(UFortKismetLibrary* FortKismetLibrary, FFrame& Stack, AFortAIGoalManager** Ret)
{
    UObject* WorldContextObject;

    Stack.StepCompiledIn(&WorldContextObject);
    Stack.IncrementCode();

    if (AActor* WorldContext = Cast<AActor>(WorldContextObject))
    {
        if (UWorld* World = WorldContext->GetWorld())
            return *Ret = Cast<AFortGameModeZone>(World->AuthorityGameMode)->AIGoalManager;
    }

    return *Ret = GGameMode->AIGoalManager;
}

AFortPickup* FortKismetLibrary::K2_SpawnPickupInWorld(UFortKismetLibrary* FortKismetLibrary, FFrame& Stack, AFortPickup** Ret)
{
    UObject* WorldContextObject;
    UFortWorldItemDefinition* ItemDefinition;
    int32 NumberToSpawn;
    FVector Position;
    FVector Direction;
    int32 OverrideMaxStackCount;
    bool bToss;
    bool bRandomRotation;
    bool bBlockedFromAutoPickup;

    Stack.StepCompiledIn(&WorldContextObject);
    Stack.StepCompiledIn(&ItemDefinition);
    Stack.StepCompiledIn(&NumberToSpawn);
    Stack.StepCompiledIn(&Position);
    Stack.StepCompiledIn(&Direction);
    Stack.StepCompiledIn(&OverrideMaxStackCount);
    Stack.StepCompiledIn(&bToss);
    Stack.StepCompiledIn(&bRandomRotation);
    Stack.StepCompiledIn(&bBlockedFromAutoPickup);
    Stack.IncrementCode();

    return *Ret = AFortPickup::SpawnPickup(FFortItemEntry(ItemDefinition, NumberToSpawn, 0), Position, NumberToSpawn, EFortPickupSourceTypeFlag::Other, -1, bRandomRotation, bToss);
}

bool FortKismetLibrary::PickLootDrops(UFortKismetLibrary* FortKismetLibrary, FFrame& Stack, bool* Ret)
{
    UObject* WorldContextObject;
    FName TierGroupName;
    int32 WorldLevel;
    int32 ForcedLootTier;

    Stack.StepCompiledIn(&WorldContextObject);

    auto& OutLootToDrop = Stack.StepCompiledInRef<TArray<FFortItemEntry>>();

    Stack.StepCompiledIn(&TierGroupName);
    Stack.StepCompiledIn(&WorldLevel);
    Stack.StepCompiledIn(&ForcedLootTier);

    Stack.IncrementCode();

    FortLootPackage::PickLootDrops(&OutLootToDrop, ForcedLootTier, TierGroupName);

    for (FFortItemEntry& LootDrop : OutLootToDrop)
    {
        if (UFortWeaponRangedItemDefinition* WeaponRangedItemDefinition = Cast<UFortWeaponRangedItemDefinition>(LootDrop.ItemDefinition))
        {
            FFortRangedWeaponStats OutRow;
            UFortKismetLibrary::GetRangedWeaponStatsRow(WeaponRangedItemDefinition->WeaponStatHandle, &OutRow);

            LootDrop.LoadedAmmo = OutRow.ClipSize;
        }
    }

    return *Ret = OutLootToDrop.Num() > 0;
}

void FortKismetLibrary::Setup()
{
    Utils::Exec(TEXT("/Script/FortniteGame.FortKismetLibrary.GetAIGoalManager"), GetAIGoalManager);
    Utils::Exec(TEXT("/Script/FortniteGame.FortKismetLibrary.K2_SpawnPickupInWorld"), K2_SpawnPickupInWorld);
    Utils::Exec(TEXT("/Script/FortniteGame.FortKismetLibrary.PickLootDrops"), PickLootDrops);
}