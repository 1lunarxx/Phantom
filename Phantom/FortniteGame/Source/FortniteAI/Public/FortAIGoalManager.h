#pragma once
#include "pch.h"
#include "Utils.h"

class FortAIGoalManager
{
private:
	static AFortAIGoalManager* GetCurrent(UWorld* World);
public:
	static void Setup();
};