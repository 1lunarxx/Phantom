#include "pch.h"
#include "FortniteGame/Public/FortKismetLibrary.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

AFortAIGoalManager* FortKismetLibrary::GetAIGoalManager(UFortKismetLibrary* Context, FFrame* Stack, AFortAIGoalManager** Result)
{
    UObject* WorldContextObject;

    Stack->StepCompiledIn(&WorldContextObject);
    Stack->IncrementCode();

    return *Result = GWorld->GetGameMode()->AIGoalManager;
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

    FFortItemEntry ItemEntry = FFortItemEntry(ItemDefinition, NumberToSpawn, 0);

    if (UFortWeaponRangedItemDefinition* WeaponRangedItemDefinition = Cast<UFortWeaponRangedItemDefinition>(ItemDefinition))
    {
        FFortRangedWeaponStats OutRow;
        UFortKismetLibrary::GetRangedWeaponStatsRow(WeaponRangedItemDefinition->WeaponStatHandle, &OutRow);

        ItemEntry.LoadedAmmo = OutRow.ClipSize;
    }

    return *Result = AFortPickup::CreateFromData(FortPickupCreationData(GWorld, &ItemEntry, Position, FRotator(), NULL, NULL, NULL, EFortPickupSourceTypeFlag::Other, 0, true, false));
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

    return *Result = UFortLootPackage::PickLootDrops(&OutLootToDrop, ForcedLootTier, TierGroupName);
}

void FortKismetLibrary::Setup()
{
    Utils::ExecHook(TEXT("/Script/FortniteGame.FortKismetLibrary.GetAIGoalManager"), GetAIGoalManager);
    Utils::ExecHook(TEXT("/Script/FortniteGame.FortKismetLibrary.K2_SpawnPickupInWorld"), K2_SpawnPickupInWorld);
    Utils::ExecHook(TEXT("/Script/FortniteGame.FortKismetLibrary.PickLootDrops"), PickLootDrops);
}