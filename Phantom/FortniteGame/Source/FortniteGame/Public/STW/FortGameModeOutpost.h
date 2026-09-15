#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeOutpost
{
	class Originals
	{
	public:
		static inline void (*InitGameState)(AFortGameModeOutpost* FortGameModeOutpost);
		static inline void (*HandleStartingNewPlayer)(AFortGameModeOutpost* FortGameModeOutpost, APlayerController* NewPlayer);
	};

private:
	static void InitGameState(AFortGameModeOutpost* FortGameModeOutpost);
	static void HandleStartingNewPlayer(AFortGameModeOutpost* FortGameModeOutpost, APlayerController* NewPlayer);
public:
	static void Setup();
};