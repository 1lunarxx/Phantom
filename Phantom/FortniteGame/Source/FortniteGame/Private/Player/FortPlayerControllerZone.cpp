#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerControllerZone.h"

void FortPlayerControllerZone::ServerAcknowledgePossession_Implementation(AFortPlayerControllerZone* FortPlayerControllerZone, APawn* P)
{
	FortPlayerControllerZone->AcknowledgedPawn = P;

	if (AFortPlayerState* PS = Cast<AFortPlayerState>(FortPlayerControllerZone->PlayerState))
	{
		if (FortPlayerControllerZone->MyFortPawn != NULL)
			PS->ApplyCharacterCustomization(FortPlayerControllerZone->MyFortPawn);

		if (UFortAssetManager* AssetManager = Cast<UFortAssetManager>(UEngine::GetEngine()->AssetManager))
		{
			UFortAbilitySet* GenericPlayerAbilitySet = AssetManager->GameData->GenericPlayerAbilitySet.Get();

			if (GenericPlayerAbilitySet != NULL)
			{
				TScriptInterface<IAbilitySystemInterface> AbilitySystemInterfaceActor;

				AbilitySystemInterfaceActor.ObjectPointer = PS;
				AbilitySystemInterfaceActor.InterfacePointer = PS->GetInterfaceAddress<IAbilitySystemInterface>();

				UFortKismetLibrary::EquipFortAbilitySet(AbilitySystemInterfaceActor, GenericPlayerAbilitySet, NULL);
			}
		}
	}
}

void FortPlayerControllerZone::Setup()
{
	Utils::Virtual(AFortPlayerControllerZone::GetDefaultObj(), 0x828 / 8, ServerAcknowledgePossession_Implementation);
}