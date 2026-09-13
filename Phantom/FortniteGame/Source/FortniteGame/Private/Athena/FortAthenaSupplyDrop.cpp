#include "pch.h"
#include "FortniteGame/Public/Athena/FortAthenaSupplyDrop.h"

AFortPickup* FortAthenaSupplyDrop::SpawnPickup(AFortAthenaSupplyDrop* Context, FFrame* Stack, AFortPickup** Result)
{
    UFortWorldItemDefinition* ItemDefinition;
    int32 NumberToSpawn;
    AFortPawn* TriggeringPawn;
    FVector Position;
    FVector Direction;

    Stack->StepCompiledIn(&ItemDefinition);
    Stack->StepCompiledIn(&NumberToSpawn);
    Stack->StepCompiledIn(&TriggeringPawn);
    Stack->StepCompiledIn(&Position);
    Stack->StepCompiledIn(&Direction);

    Stack->IncrementCode();

    FFortItemEntry ItemEntry = FFortItemEntry(ItemDefinition, NumberToSpawn, 0);

    if (UFortWeaponRangedItemDefinition* WeaponRangedItemDefinition = Cast<UFortWeaponRangedItemDefinition>(ItemDefinition))
    {
        FFortRangedWeaponStats OutRow;
        UFortKismetLibrary::GetRangedWeaponStatsRow(WeaponRangedItemDefinition->WeaponStatHandle, &OutRow);

        ItemEntry.LoadedAmmo = OutRow.ClipSize;
    }

    return *Result = AFortPickup::SpawnPickup(ItemEntry, Position, NumberToSpawn, EFortPickupSourceTypeFlag::Container_SupplyDrop, 3);
}

void FortAthenaSupplyDrop::Setup()
{
    Utils::Exec(TEXT("/Script/FortniteGame.FortAthenaSupplyDrop.SpawnPickup"), SpawnPickup);
}