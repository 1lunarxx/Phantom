#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerController.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

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

void FortPlayerController::ServerAttemptInteract(AFortPlayerController* FortPlayerController, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, ETInteractionType InteractType, UObject* OptionalObjectData)
{
	Originals::ServerAttemptInteract(FortPlayerController, ReceivingActor, InteractComponent, InteractType, OptionalObjectData);

	if (ABuildingContainer* BuildingContainer = Cast<ABuildingContainer>(ReceivingActor))
	{
		FortLootPackage::SpawnLoot(BuildingContainer);
	}
}

void FortPlayerController::Setup()
{
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0xFA0 / 8, ServerExecuteInventoryItem_Implementation);
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x1218 / 8, ServerAttemptInteract, (void**)&Originals::ServerAttemptInteract);
}