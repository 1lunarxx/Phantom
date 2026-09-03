#pragma once
#include "pch.h"
#include "Utils.h"

class FortKismetLibrary
{
private:
	static AFortAIGoalManager* GetAIGoalManager(UFortKismetLibrary* FortKismetLibrary, FFrame& Stack, AFortAIGoalManager** Ret);
public:
	static void Setup();
};