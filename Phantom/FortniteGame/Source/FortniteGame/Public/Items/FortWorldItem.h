#pragma once
#include "pch.h"
#include "Utils.h"

class FortWorldItem
{
private:
	static void SetLoadedAmmo(UFortWorldItem* FortWorldItem, int InCount);
	static void SetDurability(UFortWorldItem* FortWorldItem, float InDurability);
public:
	static void Setup();
};