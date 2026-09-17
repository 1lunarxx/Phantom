#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeZone
{
private:
	static void CreateAIDirector(AFortGameModeZone* FortGameModeZone);
	static void FinishWorldInitialization(AFortGameModeZone* FortGameModeZone, AFortWorldManager* WorldManager);

	static APawn* SpawnDefaultPawnFor_Implementation(AFortGameModeZone* FortGameModeZone, AController* NewPlayer, AActor* StartSpot);
public:
	static void Setup();
};