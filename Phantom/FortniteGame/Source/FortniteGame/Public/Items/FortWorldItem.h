#pragma once
#include "pch.h"
#include "Utils.h"

class FortWorldItem
{
private:
	static bool SetLoadedAmmo(UFortWorldItem* FortWorldItem, int InCount);
public:
	static void Setup();
};