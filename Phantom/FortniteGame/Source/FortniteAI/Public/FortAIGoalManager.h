#pragma once
#include "pch.h"
#include "Utils.h"

class FortAIGoalManager
{
private:
	static AFortAIGoalManager* GetCurrent(UObject* WorldContextObject);
public:
	static void Setup();
};