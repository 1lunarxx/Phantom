#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeZone
{
	class Originals
	{
	public:
		static inline void (*HandleStartingNewPlayer_Implementation)(AFortGameModeZone* FortGameModeZone, AFortPlayerController* NewPlayer);
	};

private:
	static void CreateAIDirector(AFortGameModeZone* FortGameModeZone);
	static void FinishWorldInitialization(AFortGameModeZone* FortGameModeZone, AFortWorldManager* WorldManager);
	static void HandleStartingNewPlayer_Implementation(AFortGameModeZone* FortGameModeZone, AFortPlayerController* NewPlayer);
public:
	static void Setup();
};