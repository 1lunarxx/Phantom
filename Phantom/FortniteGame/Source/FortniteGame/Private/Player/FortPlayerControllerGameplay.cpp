#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerControllerGameplay.h"

void FortPlayerControllerGameplay::GiveItemToInventoryOwner_StartGhostMode(TScriptInterface<IFortInventoryOwnerInterface> InventoryOwner, UFortWorldItemDefinition* ItemDefinition, int NumberToGive)
{
	IFortInventoryOwnerInterface* FortInventoryOwnerInterface = (IFortInventoryOwnerInterface*)InventoryOwner.GetInterfaceRef();

	if (FortInventoryOwnerInterface == NULL)
		return;

	AFortPlayerController* FortPlayerController = FortInventoryOwnerInterface->GetOwner<AFortPlayerController>();

	if (FortPlayerController != NULL)
	{
		UFortWorldItem* WorldItem = FortPlayerController->WorldInventory->AddItem(ItemDefinition, NumberToGive);

		if (WorldItem != NULL)
		{
			if (UFortGadgetItemDefinition* FortGadgetItemDefinition = Cast<UFortGadgetItemDefinition>(ItemDefinition))
			{
				if (UFortWeaponItemDefinition* FortWeaponItemDefinition = Cast<UFortWeaponItemDefinition>(FortGadgetItemDefinition->GetDecoItemDefinition()))
					FortWeaponItemDefinition->ServerExecute(WorldItem, FortPlayerController);
			}
		}
	}
}

void FortPlayerControllerGameplay::EndGhostMode(AFortPlayerControllerGameplay* Context, FFrame* Stack)
{
	Stack->IncrementCode();

	if (AFortInventory* WorldInventory = Context->GetWorldInventory())
	{
		UFortWorldItem* WorldItem = WorldInventory->FindExistingItemForDefinition(Context->GhostModeRepData.GhostModeItemDef);

		if (WorldItem != NULL)
			WorldInventory->RemoveItem(WorldItem->ItemEntry.ItemGuid);

		Context->GhostModeRepData.bInGhostMode = false;
		Context->CheckGhostModeItemRemoved(Context->GhostModeRepData.GhostModeItemDef);
	}
}

void FortPlayerControllerGameplay::Setup()
{
	Utils::Exec(TEXT("/Script/FortniteGame.FortPlayerControllerGameplay.EndGhostMode"), EndGhostMode);

	Utils::Rel32(InSDKUtils::GetImageBase() + 0x132750F, GiveItemToInventoryOwner_StartGhostMode);
}