#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerController
{
private:
	static void ServerExecuteInventoryItem_Implementation(AFortPlayerController* FortPlayerController, FGuid& ItemGuid);
	static void ServerAttemptInventoryDrop_Implementation(AFortPlayerController* FortPlayerController, FGuid* ItemGuid, int Count, bool bTrash);
	static void ServerCheat_Implementation(AFortPlayerController* FortPlayerController, FString& Msg);
	static void ServerTeleportToReticle_Implementation(AFortPlayerController* FortPlayerController, FVector* TeleportLocation);

	static void ServerPlayEmoteItem_Implementation(AFortPlayerController* FortPlayerController, UFortMontageItemDefinitionBase* EmoteAsset);
	static void ServerPlayEmoteItem_Internal(AFortPlayerController* FortPlayerController, UFortMontageItemDefinitionBase* EmoteAsset);

	static void ServerCreateBuildingActor_Implementation(AFortPlayerController* FortPlayerController, FBuildingClassData& BuildingClassData, FVector_NetQuantize10& BuildLoc, FRotator& BuildRot, bool bMirrored, float SyncKey);
	static void ServerBeginEditingBuildingActor_Implementation(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToEdit);
	static void ServerEditBuildingActor_Implementation(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, uint8 RotationIterations, bool bMirrored);
	static void ServerEndEditingBuildingActor_Implementation(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToEdit);
	static void ServerRepairBuildingActor_Implementation(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToRepair);

	static void ServerCombineInventoryItems_Implementation(AFortPlayerController* FortPlayerController, FGuid& TargetItemGuid, FGuid& SourceItemGuid);

	static void DropItemsOnPawnDestruction(AFortPlayerController* FortPlayerController, AFortPlayerController::EPawnDestructionReason DestructionReason, const FGameplayTagContainer* ContextualTags, AFortPawn* DestructionPawn);
	static bool FixUpCreateBuildingClassData(AFortPlayerController* FortPlayerController, FBuildingClassData* BuildingActorData);

	static AActor* SpawnToyInstance(AFortPlayerController* Context, FFrame* Stack, AActor** Result);
public:
	static void Setup();
};