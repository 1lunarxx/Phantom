#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeAthena
{
	class Originals
	{
	public:
		static inline void (*InitGameState)(AFortGameModeAthena* FortGameModeAthena);
		static inline void (*HandleStartingNewPlayer)(AFortGameModeAthena* FortGameModeAthena, APlayerController* NewPlayer);

		static inline APawn* (*SpawnDefaultPawnFor_Implementation)(AFortGameModeAthena* FortGameModeAthena, AController* NewPlayer, AActor* StartSpot);
	};

private:
	static void InitGameState(AFortGameModeAthena* FortGameModeAthena);
	static void HandleStartingNewPlayer(AFortGameModeAthena* FortGameModeAthena, APlayerController* NewPlayer);

	static APawn* SpawnDefaultPawnFor_Implementation(AFortGameModeAthena* FortGameModeAthena, AController* NewPlayer, AActor* StartSpot);
public:
	static void Setup();
};