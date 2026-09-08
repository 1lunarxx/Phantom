#include "pch.h"
#include "FortniteGame/Public/Building/BuildingActor.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

void BuildingActor::OnDeathServer(ABuildingActor* BuildingActor, double Damage, const FGameplayTagContainer* DamageTags, FVector* Momentum, const FHitResult* HitInfo, AController* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle* EffectContext)
{
	Originals::OnDeathServer(BuildingActor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

	if (AAthenaBarrierObjective* BarrierObjective = Cast<AAthenaBarrierObjective>(BuildingActor))
	{
		// TODO: not be lazy and find something like GetMutatorByClass in this season

/*		AFortAthenaMutator_Barrier* BarrierMutator = Cast<AFortAthenaMutator_Barrier>(GGameState->GetMutatorByClass(GGameState, AFortAthenaMutator_Barrier::StaticClass()));

		if (BarrierMutator != NULL)
			BarrierMutator->OnObjectiveDestroyed(BarrierObjective);*/
	}
}

void BuildingActor::BeginPlay(ABGAConsumableSpawner* BGAConsumableSpawner)
{
    Originals::BeginPlay(BGAConsumableSpawner);

    FortLootPackage::PickLootDrops(&BGAConsumableSpawner->ConsumablesToSpawn, GGameState->WorldLevel, BGAConsumableSpawner->SpawnLootTierGroup);

    for (FFortItemEntry& ConsumableToSpawn : BGAConsumableSpawner->ConsumablesToSpawn)
    {
        UBGAConsumableWrapperItemDefinition* BGAConsumableWrapperItemDefinition = Cast<UBGAConsumableWrapperItemDefinition>(ConsumableToSpawn.ItemDefinition);

        if (BGAConsumableWrapperItemDefinition == NULL)
            continue;

        FVector Location = BGAConsumableSpawner->K2_GetActorLocation();
        FRotator Rotation = BGAConsumableSpawner->K2_GetActorRotation();

        GWorld->SpawnActor(BGAConsumableWrapperItemDefinition->ConsumableClass.Get(), &Location, &Rotation, NULL);

        break;
    }
}

void BuildingActor::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x149E410, OnDeathServer, (void**)&Originals::OnDeathServer);

    Utils::Virtual(ABGAConsumableSpawner::GetDefaultObj()->VTable, 0x2F8 / 8, BeginPlay, (void**)&Originals::BeginPlay);
}