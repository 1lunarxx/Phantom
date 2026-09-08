#pragma once
#include "pch.h"
#include "Utils.h"

class BuildingActor
{
	class Originals
	{
	public:
		static inline void (*OnDeathServer)(ABuildingActor* BuildingActor, double Damage, const FGameplayTagContainer* DamageTags, FVector* Momentum, const FHitResult* HitInfo, AController* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle* EffectContext);
		static inline void (*BeginPlay)(ABGAConsumableSpawner* BGAConsumableSpawner);
	};

private:
	static void OnDeathServer(ABuildingActor* BuildingActor, double Damage, const FGameplayTagContainer* DamageTags, FVector* Momentum, const FHitResult* HitInfo, AController* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle* EffectContext);
	static void BeginPlay(ABGAConsumableSpawner* BGAConsumableSpawner);
public:
	static void Setup();
};