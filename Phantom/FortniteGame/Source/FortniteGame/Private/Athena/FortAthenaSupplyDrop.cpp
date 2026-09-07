#include "pch.h"
#include "FortniteGame/Public/Athena/FortAthenaSupplyDrop.h"

AFortPickup* FortAthenaSupplyDrop::SpawnPickup(AFortAthenaSupplyDrop* FortAthenaSupplyDrop, FFrame& Stack, AFortPickup** Ret)
{
    UFortWorldItemDefinition* ItemDefinition;
    int32 NumberToSpawn;
    AFortPawn* TriggeringPawn;
    FVector Position;
    FVector Direction;

    Stack.StepCompiledIn(&ItemDefinition);
    Stack.StepCompiledIn(&NumberToSpawn);
    Stack.StepCompiledIn(&TriggeringPawn);
    Stack.StepCompiledIn(&Position);
    Stack.StepCompiledIn(&Direction);

    Stack.IncrementCode();

    return *Ret = AFortPickup::SpawnPickup(FFortItemEntry(ItemDefinition, NumberToSpawn, 0), Position, NumberToSpawn, EFortPickupSourceTypeFlag::Container_SupplyDrop, 3);
}

void FortAthenaSupplyDrop::Setup()
{
    Utils::Exec(TEXT("/Script/FortniteGame.FortAthenaSupplyDrop.SpawnPickup"), SpawnPickup);
}