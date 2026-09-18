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

    return *Result = AFortPickup::CreateFromData(FortPickupCreationData(GWorld, &ItemEntry, Position, FRotator(), NULL, NULL, NULL, EFortPickupSourceTypeFlag::Container_SupplyDrop, 0, true, false));
}

void FortAthenaSupplyDrop::Setup()
{
    Utils::ExecHook(TEXT("/Script/FortniteGame.FortAthenaSupplyDrop.SpawnPickup"), SpawnPickup);
}