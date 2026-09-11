#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerController
{
	class Originals
	{
	public:
		static inline void (*ClientRestart_Implementation)(AFortPlayerController* FortPlayerController, APawn* NewPawn);
	};

private:
	static void ServerExecuteInventoryItem_Implementation(AFortPlayerController* FortPlayerController, FGuid& ItemGuid);
	static void ServerAttemptInventoryDrop_Implementation(AFortPlayerController* FortPlayerController, FGuid* ItemGuid, int Count, bool bTrash);
	static void ServerCheat_Implementation(AFortPlayerController* FortPlayerController, FString& Msg);
	static void ServerTeleportToReticle_Implementation(AFortPlayerController* FortPlayerController, FVector* TeleportLocation);
	static void ClientRestart_Implementation(AFortPlayerController* FortPlayerController, APawn* NewPawn);

	static void ServerPlayEmoteItem_Implementation(AFortPlayerController* FortPlayerController, UFortMontageItemDefinitionBase* EmoteAsset);
	static void ServerPlayEmoteItem_Internal(AFortPlayerController* FortPlayerController, UFortMontageItemDefinitionBase* EmoteAsset);

	static bool FixUpCreateBuildingClassData(AFortPlayerController* FortPlayerController, FBuildingClassData* BuildingActorData);

	static void ServerCreateBuildingActor(AFortPlayerController* FortPlayerController, FBuildingClassData& BuildingClassData, FVector_NetQuantize10& BuildLoc, FRotator& BuildRot, bool bMirrored, float SyncKey);
	static void ServerBeginEditingBuildingActor(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToEdit);
	static void ServerEditBuildingActor(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, uint8 RotationIterations, bool bMirrored);
	static void ServerEndEditingBuildingActor(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToEdit);
	static void ServerRepairBuildingActor(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToRepair);
	static void DropItemsOnPawnDestruction(AFortPlayerController* FortPlayerController, AFortPlayerController::EPawnDestructionReason DestructionReason, const FGameplayTagContainer* ContextualTags, AFortPawn* DestructionPawn);
public:
	static void Setup();
};