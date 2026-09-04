#pragma once
#include "pch.h"
#include "Utils.h"

class BuildingSMActor
{
private:
	static void AttemptSpawnResources(ABuildingSMActor* BuildingSMActor, AFortPlayerPawn* InstigatorPawn, float ActualDamageDealt, bool bJustHitWeakspot);
public:
	static void Setup();
};