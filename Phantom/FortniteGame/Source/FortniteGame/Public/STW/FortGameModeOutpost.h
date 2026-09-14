#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeOutpost
{
	class Originals
	{
	public:
		static inline void (*InitGameState)(AFortGameModeOutpost* FortGameModeOutpost);
	};

private:
	static void InitGameState(AFortGameModeOutpost* FortGameModeOutpost);
public:
	static void Setup();
};