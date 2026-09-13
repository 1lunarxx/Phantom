#include "pch.h"
#include "FortniteGame/Public/FortKismetLibrary.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

AFortAIGoalManager* FortKismetLibrary::GetAIGoalManager(UFortKismetLibrary* Context, FFrame* Stack, AFortAIGoalManager** Result)
{
    UObject* WorldContextObject;

    Stack->StepCompiledIn(&WorldContextObject);
    Stack->IncrementCode();

    if (AActor* WorldContext = Cast<AActor>(WorldContextObject))
    {
        if (UWorld* World = WorldContext->GetWorld())
            return *Result = Cast<AFortGameModeZone>(World->AuthorityGameMode)->AIGoalManager;
    }

    return *Result = GGameMode->AIGoalManager;
}

AFortPickup* FortKismetLibrary::K2_SpawnPickupInWorld(UFortKismetLibrary* Context, FFrame* Stack, AFortPickup** Result)
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

    Stack->StepCompiledIn(&WorldContextObject);
    Stack->StepCompiledIn(&ItemDefinition);
    Stack->StepCompiledIn(&NumberToSpawn);
    Stack->StepCompiledIn(&Position);
    Stack->StepCompiledIn(&Direction);
    Stack->StepCompiledIn(&OverrideMaxStackCount);
    Stack->StepCompiledIn(&bToss);
    Stack->StepCompiledIn(&bRandomRotation);
    Stack->StepCompiledIn(&bBlockedFromAutoPickup);
    Stack->IncrementCode();

    return *Result = AFortPickup::SpawnPickup(FFortItemEntry(ItemDefinition, NumberToSpawn, 0), Position, NumberToSpawn, EFortPickupSourceTypeFlag::Other, -1, bRandomRotation, bToss);
}

bool FortKismetLibrary::PickLootDrops(UFortKismetLibrary* Context, FFrame* Stack, bool* Result)
{
    UObject* WorldContextObject;
    FName TierGroupName;
    int32 WorldLevel;
    int32 ForcedLootTier;

    Stack->StepCompiledIn(&WorldContextObject);

    auto& OutLootToDrop = Stack->StepCompiledInRef<TArray<FFortItemEntry>>();

    Stack->StepCompiledIn(&TierGroupName);
    Stack->StepCompiledIn(&WorldLevel);
    Stack->StepCompiledIn(&ForcedLootTier);

    Stack->IncrementCode();

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

    return *Result = OutLootToDrop.Num() > 0;
}

void FortKismetLibrary::Setup()
{
    Utils::Exec(TEXT("/Script/FortniteGame.FortKismetLibrary.GetAIGoalManager"), GetAIGoalManager);
    Utils::Exec(TEXT("/Script/FortniteGame.FortKismetLibrary.K2_SpawnPickupInWorld"), K2_SpawnPickupInWorld);
    Utils::Exec(TEXT("/Script/FortniteGame.FortKismetLibrary.PickLootDrops"), PickLootDrops);
}