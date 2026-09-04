#include "pch.h"
#include "FortniteGame/Public/Items/FortInventoryOwnerInterface.h"

bool FortInventoryOwnerInterface::RemoveInventoryItem(IFortInventoryOwnerInterface* FortInventoryOwnerInterface, FGuid& ItemGuid, int32 Count, bool bForceRemoval, bool bForcePersistWhenEmpty)
{
	AFortPlayerController* FortPlayerController = FortInventoryOwnerInterface->GetOwner<AFortPlayerController>();

	if (FortPlayerController == NULL)
		return false;

	AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory();

	if (WorldInventory == NULL)
		return false;

	FFortItemEntry* ItemEntry = WorldInventory->GetReplicatedItemEntry(&ItemGuid);

	if (ItemEntry == NULL)
		return false;

	int32 NewCount = UKismetMathLibrary::Min(Count, ItemEntry->Count);

	ItemEntry->Count -= NewCount;

	if (bForceRemoval || ItemEntry->Count <= 0)
		WorldInventory->RemoveItem(ItemGuid);
	else
		WorldInventory->UpdateItemEntry(ItemEntry);

	return true;
}

void FortInventoryOwnerInterface::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x12E7790, RemoveInventoryItem);
}