#include "pch.h"
#include "FortniteGame/Public/Building/BuildingActor.h"

void BuildingActor::OnDeathServer(ABuildingActor* BuildingActor, double Damage, const FGameplayTagContainer* DamageTags, FVector* Momentum, const FHitResult* HitInfo, AController* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle* EffectContext)
{
	Originals::OnDeathServer(BuildingActor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

	if (AAthenaBarrierObjective* BarrierObjective = Cast<AAthenaBarrierObjective>(BuildingActor))
	{
/*		AFortAthenaMutator_Barrier* BarrierMutator = Cast<AFortAthenaMutator_Barrier>(GGameState->GetMutatorByClass(GGameState, AFortAthenaMutator_Barrier::StaticClass()));

		if (BarrierMutator != NULL)
			BarrierMutator->OnObjectiveDestroyed(BarrierObjective);*/
	}
}

void BuildingActor::Setup()
{
	//Utils::Hook(InSDKUtils::GetImageBase() + 0x225AE10, OnDeathServer, (void**)&Originals::OnDeathServer);
}