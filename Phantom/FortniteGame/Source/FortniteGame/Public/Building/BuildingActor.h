#pragma once
#include "pch.h"
#include "Utils.h"

class BuildingActor
{
	class Originals
	{
	public:
		static inline void (*OnDeathServer)(ABuildingActor* BuildingActor, double Damage, const FGameplayTagContainer* DamageTags, FVector* Momentum, const FHitResult* HitInfo, AController* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle* EffectContext);
	};

private:
	static void OnDeathServer(ABuildingActor* BuildingActor, double Damage, const FGameplayTagContainer* DamageTags, FVector* Momentum, const FHitResult* HitInfo, AController* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle* EffectContext);
public:
	static void Setup();
};