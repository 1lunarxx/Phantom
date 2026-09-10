#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerControllerAthena
{
	class Originals
	{
	public:
		static inline void (*OnPawnDied)(AFortPlayerControllerAthena* FortPlayerControllerAthena, AFortPlayerPawn* KilledPawn, const FGameplayTagContainer* InTags, const FGameplayEffectContextHandle* EffectContext, AController* EventInstigator, AActor* DamageCauser, AController* DBNOFinisher);
	};

private:
	static void ServerRestartPlayer_Implementation(AFortPlayerControllerAthena* FortPlayerControllerAthena);
	static void ServerReturnToMainMenu_Implementation(AFortPlayerControllerAthena* FortPlayerControllerAthena);

	static void RemoveAllInventoryItems(AFortPlayerControllerAthena* FortPlayerControllerAthena);
	static void OnPawnDied(AFortPlayerControllerAthena* FortPlayerControllerAthena, AFortPlayerPawn* KilledPawn, const FGameplayTagContainer* InTags, const FGameplayEffectContextHandle* EffectContext, AController* EventInstigator, AActor* DamageCauser, AController* DBNOFinisher);
public:
	static void Setup();
};