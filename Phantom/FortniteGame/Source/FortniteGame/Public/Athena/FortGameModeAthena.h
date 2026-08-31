#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeAthena
{
	class Originals
	{
	public:
		static inline void (*FinishWorldInitialization)(AFortGameModeAthena* FortGameModeAthena, AFortWorldManager* WorldManager);
		static inline void (*InitGameState)(AFortGameModeAthena* FortGameModeAthena);

		static inline APawn* (*SpawnDefaultPawnFor_Implementation)(AFortGameModeAthena* FortGameModeAthena, AController* NewPlayer, AActor* StartSpot);
	};

private:
	static void FinishWorldInitialization(AFortGameModeAthena* FortGameModeAthena, AFortWorldManager* WorldManager);
	static void InitGameState(AFortGameModeAthena* FortGameModeAthena);

	static APawn* SpawnDefaultPawnFor_Implementation(AFortGameModeAthena* FortGameModeAthena, AController* NewPlayer, AActor* StartSpot);
public:
	static void Setup();
};