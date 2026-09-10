#pragma once
#include "pch.h"
#include "Utils.h"

class CheatManager
{
private:
	static void Slomo(UCheatManager* CheatManager, float NewTimeDilation);
public:
	static void Setup();
};