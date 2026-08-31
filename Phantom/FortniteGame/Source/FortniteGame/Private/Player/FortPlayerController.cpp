#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerController.h"

void FortPlayerController::ServerExecuteInventoryItem_Implementation(AFortPlayerController* FortPlayerController, FGuid* ItemGuid)
{
	if (AFortPlayerPawn* MyFortPawn = FortPlayerController->MyFortPawn)
	{
		UFortItem* InventoryItem = FortPlayerController->K2_GetInventoryItemWithGuid(*ItemGuid);

		if (InventoryItem != NULL)
			MyFortPawn->EquipWeaponDefinition(Cast<UFortWeaponItemDefinition>(InventoryItem->GetItemDefinitionBP()), *ItemGuid);
	}
}

void FortPlayerController::Setup()
{
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0xFA0 / 8, ServerExecuteInventoryItem_Implementation);
}