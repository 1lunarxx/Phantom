#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeOutpost
{
	class Originals
	{
	public:
		static inline void (*InitGameState)(AFortGameModeOutpost* FortGameModeOutpost);
		static inline void (*FinishWorldInitialization)(AFortGameModeOutpost* FortGameModeOutpost, AFortWorldManager* WorldManager);
	};

private:
	static void CreateAIDirector(AFortGameModeOutpost* FortGameModeOutpost);
	static void InitGameState(AFortGameModeOutpost* FortGameModeOutpost);
	static void FinishWorldInitialization(AFortGameModeOutpost* FortGameModeOutpost, AFortWorldManager* WorldManager);

	static APawn* SpawnDefaultPawnFor_Implementation(AFortGameModeOutpost* FortGameModeOutpost, AController* NewPlayer, AActor* StartSpot);
public:
	static void Setup();
};