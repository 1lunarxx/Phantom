#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerControllerGameplay.h"

void FortPlayerControllerGameplay::GiveItemToInventoryOwner_StartGhostMode(TScriptInterface<IFortInventoryOwnerInterface> InventoryOwner, UFortWorldItemDefinition* ItemDefinition, int NumberToGive)
{
	IFortInventoryOwnerInterface* FortInventoryOwnerInterface = (IFortInventoryOwnerInterface*)InventoryOwner.GetInterfaceRef();

	if (FortInventoryOwnerInterface == NULL)
		return;

	AFortPlayerController* FortPlayerController = FortInventoryOwnerInterface->GetOwner<AFortPlayerController>();

	if (FortPlayerController != NULL)
		FortPlayerController->WorldInventory->AddItem(ItemDefinition, NumberToGive);
}

void FortPlayerControllerGameplay::EndGhostMode(AFortPlayerControllerGameplay* FortPlayerControllerGameplay, FFrame& Stack)
{
	Stack.IncrementCode();

	if (AFortInventory* WorldInventory = FortPlayerControllerGameplay->GetWorldInventory())
	{
		UFortWorldItem* WorldItem = WorldInventory->FindExistingItemForDefinition(FortPlayerControllerGameplay->GhostModeRepData.GhostModeItemDef);

		if (WorldItem != NULL)
			WorldInventory->RemoveItem(WorldItem->ItemEntry.ItemGuid);
	}

	FortPlayerControllerGameplay->GhostModeRepData.bInGhostMode = false;
	FortPlayerControllerGameplay->CheckGhostModeItemRemoved(FortPlayerControllerGameplay->GhostModeRepData.GhostModeItemDef);
}

void FortPlayerControllerGameplay::Setup()
{
	Utils::Rel32(InSDKUtils::GetImageBase() + 0x132750F, GiveItemToInventoryOwner_StartGhostMode);
	Utils::Exec(TEXT("/Script/FortniteGame.FortPlayerControllerGameplay.EndGhostMode"), EndGhostMode);
}