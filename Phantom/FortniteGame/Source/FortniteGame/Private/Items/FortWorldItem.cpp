#include "pch.h"
#include "FortniteGame/Public/Items/FortWorldItem.h"
#include "Core/Public/Math/UnrealMathUtility.h"

void FortWorldItem::SetLoadedAmmo(UFortWorldItem* FortWorldItem, int InCount)
{
	FortWorldItem->ItemEntry.SetLoadedAmmo(InCount);
}

void FortWorldItem::SetDurability(UFortWorldItem* FortWorldItem, float InDurability)
{
	FortWorldItem->ItemEntry.SetDurability(InDurability);
}

void FortWorldItem::Setup()
{
	Utils::Virtual(UFortWorldItem::GetDefaultObj(), 0x4A0 / 8, SetLoadedAmmo);
	//Utils::Virtual(UFortWorldItem::GetDefaultObj(), 0x478 / 8, SetDurability);
}