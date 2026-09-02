#pragma once
#include "pch.h"
#include "Utils.h"

class FortAIDirector
{
private:
	static AFortAIDirector* GetCurrent(UWorld* World);
public:
	static void Setup();
};