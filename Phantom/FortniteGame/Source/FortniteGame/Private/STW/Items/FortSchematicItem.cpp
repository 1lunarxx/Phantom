#include "pch.h"
#include "FortniteGame/Public/STW/Items/FortSchematicItem.h"

int32 FortSchematicItem::ServerCraftSchematic(UFortSchematicItem* FortSchematicItem, AFortPlayerController* Instigator, int32 PostCraftSlot, int32 CraftAmount, EFortItemTier RequestedTier)
{
	if (Instigator == NULL || Instigator->Role != ENetRole::ROLE_Authority)
		return -1;

	UFortSchematicItemDefinition* FortSchematicItemDefinition = Cast<UFortSchematicItemDefinition>(FortSchematicItem->ItemDefinition);

	if (FortSchematicItemDefinition == NULL)
		return -1;

	if (CraftAmount <= 0)
		CraftAmount = FortSchematicItemDefinition->GetQuantityProduced();

	FRecipe Recipe = FortSchematicItemDefinition->GetRecipe();

	for (const FFortItemQuantityPair& RecipeCost : Recipe.RecipeCosts)
	{
		UFortIngredientItemDefinition* FortIngredientItemDefinition = Cast<UFortIngredientItemDefinition>(UKismetSystemLibrary::GetObjectFromPrimaryAssetId(RecipeCost.ItemPrimaryAssetId));

		if (FortIngredientItemDefinition == NULL)
			continue;

		UFortWorldItem* ExistingItem = Instigator->WorldInventory->FindExistingItemForDefinition(FortIngredientItemDefinition);

		if (ExistingItem == NULL)
			return -1;

		Instigator->WorldInventory->RemoveItem(ExistingItem->ItemEntry.ItemGuid, RecipeCost.Quantity);
	}

	if (UFortWorldItemDefinition* ResultWorldItemDefinition = FortSchematicItemDefinition->GetResultWorldItemDefinition())
	{
		if (ResultWorldItemDefinition->Tier != RequestedTier)
			ResultWorldItemDefinition->Tier = RequestedTier;

		Instigator->WorldInventory->AddItemStack(ResultWorldItemDefinition, CraftAmount);
	}

	return CraftAmount;
}

void FortSchematicItem::Setup()
{
	Utils::Virtual(UFortSchematicItem::GetDefaultObj(), 0x4A8 / 8, ServerCraftSchematic);
}