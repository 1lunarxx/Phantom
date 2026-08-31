#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerController.h"

void FortPlayerController::ServerExecuteInventoryItem_Implementation(AFortPlayerController* FortPlayerController, FGuid& ItemGuid)
{
	if (AFortPlayerPawn* MyFortPawn = FortPlayerController->MyFortPawn)
	{
		IFortInventoryInterface* InventoryInterface = FortPlayerController->WorldInventory->GetInterfaceAddress<IFortInventoryInterface>();

		if (InventoryInterface != NULL)
		{
			UFortWorldItem* WorldItem = InventoryInterface->GetItem(&ItemGuid);

			if (UFortWeaponItemDefinition* WeaponItemDefinition = Cast<UFortWeaponItemDefinition>(WorldItem->ItemEntry.ItemDefinition))
				WeaponItemDefinition->ServerExecute(WorldItem, FortPlayerController);
		}
	}
}

void FortPlayerController::Setup()
{
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0xFA0 / 8, ServerExecuteInventoryItem_Implementation);
}