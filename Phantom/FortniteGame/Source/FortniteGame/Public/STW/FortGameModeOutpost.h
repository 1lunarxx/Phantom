#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeOutpost
{
	class Originals
	{
	public:
		static inline void (*HandleStartingNewPlayer)(AFortGameModeOutpost* FortGameModeOutpost, APlayerController* NewPlayer);
	};

private:
	static void HandleStartingNewPlayer(AFortGameModeOutpost* FortGameModeOutpost, APlayerController* NewPlayer);
public:
	static void Setup();
};