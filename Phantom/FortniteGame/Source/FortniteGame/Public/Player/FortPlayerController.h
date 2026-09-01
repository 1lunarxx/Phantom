#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerController
{
private:
	static void ServerExecuteInventoryItem_Implementation(AFortPlayerController* FortPlayerController, FGuid& ItemGuid);
	static bool FixUpCreateBuildingClassData(AFortPlayerController* PlayerController, FBuildingClassData* BuildingActorData);
	static void ServerCreateBuildingActor(AFortPlayerController* PlayerController, FBuildingClassData& BuildingClassData, FVector_NetQuantize10& BuildLoc, FRotator& BuildRot, bool bMirrored, float SyncKey);
	static void ServerBeginEditingBuildingActor(AFortPlayerController* PlayerController, ABuildingSMActor* BuildingActorToEdit);
	static void ServerEditBuildingActor(AFortPlayerController* PlayerController, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, uint8 RotationIterations, bool bMirrored);
	static void ServerEndEditingBuildingActor(AFortPlayerController* PlayerController, ABuildingSMActor* BuildingActorToEdit);
public:
	static void Setup();
};