#include "pch.h"
#include "FortniteGame/Public/BGAConsumableSpawner.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

void ABGAConsumableSpawner::SetupAndAttemptToSpawnConsumables()
{
    FortLootPackage::PickLootDrops(&ConsumablesToSpawn, GGameState->WorldLevel, SpawnLootTierGroup);

    for (FFortItemEntry& Consumable : ConsumablesToSpawn)
    {
        UBGAConsumableWrapperItemDefinition* BGAConsumableWrapperItemDefinition = Cast<UBGAConsumableWrapperItemDefinition>(Consumable.ItemDefinition);

        if (BGAConsumableWrapperItemDefinition == NULL)
            continue;

        GWorld->SpawnActor(BGAConsumableWrapperItemDefinition->ConsumableClass.Get(), UFortKismetLibrary::FindStaticGroundLocationAt(GWorld, K2_GetActorLocation(), this, -1000, 2500), K2_GetActorRotation(), FActorSpawnParameters(1));

        break;
    }
}

void BGAConsumableSpawner::BeginPlay(ABGAConsumableSpawner* BGAConsumableSpawner)
{
    Originals::BeginPlay(BGAConsumableSpawner);

    if (BGAConsumableSpawner->ConsumablesToSpawn.Num() <= 0)
        BGAConsumableSpawner->SetupAndAttemptToSpawnConsumables();
}

void BGAConsumableSpawner::Setup()
{
    Utils::Virtual(ABGAConsumableSpawner::GetDefaultObj()->VTable, 0x2F8 / 8, BeginPlay, (void**)&Originals::BeginPlay);
}