#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeOutpost
{
	class Originals
	{
	public:
		static inline void (*InitGameState)(AFortGameModeOutpost* FortGameModeOutpost);
		static inline void (*HandleStartingNewPlayer)(AFortGameModeAthena* FortGameModeAthena, APlayerController* NewPlayer);
	};

private:
	static void InitGameState(AFortGameModeOutpost* FortGameModeOutpost);
	static void HandleStartingNewPlayer(AFortGameModeAthena* FortGameModeAthena, APlayerController* NewPlayer);
public:
	static void Setup();
};