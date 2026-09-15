#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeZone
{
	class Originals
	{
	public:
		static inline void (*InitGameState)(AFortGameModeZone* FortGameModeZone);
		static inline void (*HandleStartingNewPlayer)(AFortGameModeZone* FortGameModeZone, APlayerController* NewPlayer);
	};

private:
	static void CreateAIDirector(AFortGameModeZone* FortGameModeZone);
	static void FinishWorldInitialization(AFortGameModeZone* FortGameModeZone, AFortWorldManager* WorldManager);
	static void InitGameState(AFortGameModeZone* FortGameModeZone);
	static void HandleStartingNewPlayer(AFortGameModeZone* FortGameModeZone, APlayerController* NewPlayer);

	static APawn* SpawnDefaultPawnFor_Implementation(AFortGameModeZone* FortGameModeZone, AController* NewPlayer, AActor* StartSpot);
public:
	static void Setup();
};