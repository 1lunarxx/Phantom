#include "pch.h"
#include "FortniteGame/Public/Items/FortWorldItem.h"
#include "Core/Public/Math/UnrealMathUtility.h"

bool FortWorldItem::SetLoadedAmmo(UFortWorldItem* FortWorldItem, int InCount)
{
	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortWorldItem->GetOwningController());

	if (FortPlayerController == NULL)
		return false;

	FortWorldItem->ItemEntry.LoadedAmmo = InCount;

	if (AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory())
		WorldInventory->UpdateItemEntry(&FortWorldItem->ItemEntry);

	return true;
}

void FortWorldItem::Setup()
{
	Utils::Virtual(UFortWorldItem::GetDefaultObj(), 0x4A0 / 8, SetLoadedAmmo);
}