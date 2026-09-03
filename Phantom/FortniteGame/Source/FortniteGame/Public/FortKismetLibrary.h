#pragma once
#include "pch.h"
#include "Utils.h"

class FortKismetLibrary
{
private:
	static AFortAIGoalManager* GetAIGoalManager(UObject* WorldContextObject);
public:
	static void Setup();
};