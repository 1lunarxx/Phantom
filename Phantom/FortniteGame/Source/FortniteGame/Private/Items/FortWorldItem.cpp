#include "pch.h"
#include "FortniteGame/Public/Items/FortWorldItem.h"
#include "Core/Public/Math/UnrealMathUtility.h"

void FortWorldItem::SetLoadedAmmo(UFortWorldItem* FortWorldItem, int InCount)
{
	FortWorldItem->ItemEntry.SetLoadedAmmo(InCount);
}

void FortWorldItem::Setup()
{
	Utils::Virtual(UFortWorldItem::GetDefaultObj(), 0x4A0 / 8, SetLoadedAmmo);
}