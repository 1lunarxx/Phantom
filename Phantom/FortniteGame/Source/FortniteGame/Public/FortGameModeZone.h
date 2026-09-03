#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameModeZone
{
private:
	static void CreateAIDirector(AFortGameModeZone* FortGameModeZone);
	static void FinishWorldInitialization(AFortGameModeZone* FortGameModeZone, AFortWorldManager* WorldManager);
public:
	static void Setup();
};