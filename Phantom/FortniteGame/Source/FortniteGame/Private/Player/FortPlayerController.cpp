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

bool FortPlayerController::FixUpCreateBuildingClassData(AFortPlayerController* PlayerController, FBuildingClassData* BuildingClassData)
{
	if (BuildingClassData == NULL || BuildingClassData->BuildingClass.Get() == NULL)
		return false;

	ABuildingSMActor* BuildingSMActor = Cast<ABuildingSMActor>(BuildingClassData->BuildingClass.Get()->DefaultObject);

	if (BuildingSMActor == NULL)
		return false;

	FPlayerBuildableClassFilter ClassFilter = FPlayerBuildableClassFilter{};

	ClassFilter.BuildingType = BuildingSMActor->BuildingType;
	ClassFilter.EditModeMetadata = BuildingSMActor->EditModePatternData;
	ClassFilter.Level = 0;
	ClassFilter.ResourceType = BuildingSMActor->ResourceType;

	TArray<TSubclassOf<ABuildingSMActor>> MatchingPlayerBuildableClasses;

	GGameState->GetPlayerBuildableClasses(&MatchingPlayerBuildableClasses, &ClassFilter);

	if (MatchingPlayerBuildableClasses.Num() <= 0)
		return false;

	TSubclassOf<ABuildingSMActor> BuildingClass;

	for (TSubclassOf<ABuildingSMActor> MatchingPlayerBuildableClass : MatchingPlayerBuildableClasses)
	{
		if (MatchingPlayerBuildableClass.Get() == NULL)
			continue;

		ABuildingSMActor* MatchingBuildingSMActor = Cast<ABuildingSMActor>(MatchingPlayerBuildableClass.Get()->DefaultObject);

		if (MatchingBuildingSMActor == BuildingSMActor)
		{
			BuildingClass = MatchingPlayerBuildableClass;
			break;
		}
	}

	if (BuildingClass.Get() != NULL)
	{
		BuildingClassData->PreviousBuildingLevel = -1;
		BuildingClassData->UpgradeLevel = 0;
		BuildingClassData->BuildingClass = BuildingClass.Get();

		return true;
	}

	return false;
}

void FortPlayerController::ServerCreateBuildingActor(AFortPlayerController* PlayerController, FBuildingClassData& BuildingClassData, FVector_NetQuantize10& BuildLoc, FRotator& BuildRot, bool bMirrored, float SyncKey)
{
	if (!FixUpCreateBuildingClassData(PlayerController, &BuildingClassData))
		return;

	if (BuildingClassData.BuildingClass != NULL)
	{
		ABuildingSMActor* BuildingSMActor = Cast<ABuildingSMActor>(BuildingClassData.BuildingClass->DefaultObject);

		if (BuildingSMActor == NULL)
			return;

		if (PlayerController->CanAffordToPlaceBuildableClass(&BuildingClassData) && PlayerController->HasRequiredBuildingLevel(BuildingClassData.PreviousBuildingLevel, BuildingSMActor->ResourceType))
		{
			TArray<ABuildingActor*> ExistingBuildings;
			EFortStructuralGridQueryResults CanPlaceBuildableClassInStructuralGrid = PlayerController->CanPlaceBuildableClassInStructuralGrid(BuildingClassData.BuildingClass, &BuildLoc, &BuildRot, bMirrored, &ExistingBuildings);

			if (CanPlaceBuildableClassInStructuralGrid == EFortStructuralGridQueryResults::CanAdd)
			{
				FActorSpawnParameters SpawnParams = FActorSpawnParameters();
				SpawnParams.SpawnCollisionHandlingOverride = 1;

				ABuildingSMActor* NewBuilding = Cast<ABuildingSMActor>(UWorld::GetWorld()->SpawnActor(BuildingClassData.BuildingClass, &BuildLoc, &BuildRot, &SpawnParams));

				if (NewBuilding != NULL)
				{
					NewBuilding->CurrentBuildingLevel = BuildingClassData.UpgradeLevel;
					NewBuilding->OnRep_CurrentBuildingLevel();

					NewBuilding->SetMirrored(bMirrored);
					NewBuilding->InitializeKismetSpawnedBuildingActor(NewBuilding, PlayerController, true);

					PlayerController->PayBuildableClassPlacementCost(&BuildingClassData);

					UFortAnalytics::FireEvent_BuildingAction(PlayerController, L"Create", BuildingSMActor, 0);

					if (GGameMode != NULL)
						GGameMode->ScoreBuildingConstruction(PlayerController, NewBuilding);

					if (AFortPlayerPawn* MyFortPawn = PlayerController->MyFortPawn)
					{
						UFortAIFunctionLibrary::MakeNoiseEventAtLocation(MyFortPawn, 0, MyFortPawn->K2_GetActorLocation());

						MyFortPawn->OnBlueprintPlace();

						if (MyFortPawn->CurrentWeapon != NULL)
						{
							AFortWeap_BuildingTool* BuildingTool = Cast<AFortWeap_BuildingTool>(MyFortPawn->CurrentWeapon);

							if (BuildingTool != NULL)
								BuildingTool->PlayWeaponFireFX_Internal(false);
						}
					}
				}
			}
		}
	}
}

void FortPlayerController::ServerBeginEditingBuildingActor(AFortPlayerController* PlayerController, ABuildingSMActor* BuildingActorToEdit)
{
	if (BuildingActorToEdit != NULL && PlayerController->MyFortPawn != NULL)
	{
		AFortPlayerStateZone* PlayerState = Cast<AFortPlayerStateZone>(PlayerController->PlayerState);

		if (PlayerState != NULL)
		{
			BuildingActorToEdit->SetEditingPlayer(PlayerState);

			UFortEditToolItemDefinition* EditToolItem = GetGameData()->EditToolItem.LoadSynchronous();
			UFortWorldItem* WorldItem = PlayerController->WorldInventory->FindExistingItemForDefinition(EditToolItem);

			if (EditToolItem != NULL && WorldItem != NULL && EditToolItem->ServerExecute(WorldItem, PlayerController))
			{
				AFortWeap_EditingTool* EditingTool = Cast<AFortWeap_EditingTool>(PlayerController->MyFortPawn->CurrentWeapon);

				if (EditingTool != NULL)
					EditingTool->SetEditActor(BuildingActorToEdit);
			}
		}
	}
}

void FortPlayerController::ServerEditBuildingActor(AFortPlayerController* PlayerController, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, uint8 RotationIterations, bool bMirrored)
{
	if (BuildingActorToEdit != NULL && BuildingActorToEdit->EditingPlayer == PlayerController->PlayerState && !BuildingActorToEdit->bDestroyed)
	{
		BuildingActorToEdit->SetEditingPlayer(NULL);
		BuildingActorToEdit->ReplaceBuildingActor(EBuildingReplacementType::BRT_Edited, NewBuildingClass, BuildingActorToEdit->GetCurrentBuildingLevel(), RotationIterations, bMirrored, PlayerController);

		UFortAnalytics::FireEvent_BuildingAction(PlayerController, L"Edit", BuildingActorToEdit, 0);

		if (GGameMode != NULL)
			GGameMode->ScoreBuildingEdit(PlayerController, BuildingActorToEdit);
	}
}

void FortPlayerController::ServerEndEditingBuildingActor(AFortPlayerController* PlayerController, ABuildingSMActor* BuildingActorToEdit)
{
	if (BuildingActorToEdit != NULL && PlayerController->MyFortPawn != NULL && BuildingActorToEdit->EditingPlayer == PlayerController->PlayerState && !BuildingActorToEdit->bDestroyed)
	{
		BuildingActorToEdit->SetEditingPlayer(NULL);

		UFortEditToolItemDefinition* EditToolItem = GetGameData()->EditToolItem.LoadSynchronous();
		UFortWorldItem* WorldItem = PlayerController->WorldInventory->FindExistingItemForDefinition(EditToolItem);

		if (EditToolItem != NULL && WorldItem != NULL && EditToolItem->ServerExecute(WorldItem, PlayerController))
		{
			AFortWeap_EditingTool* EditingTool = Cast<AFortWeap_EditingTool>(PlayerController->MyFortPawn->CurrentWeapon);

			if (EditingTool != NULL)
				EditingTool->SetEditActor(NULL);
		}
	}
}

void FortPlayerController::Setup()
{
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0xFA0 / 8, ServerExecuteInventoryItem_Implementation);

	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x10A0 / 8, ServerEditBuildingActor);
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x10C0 / 8, ServerBeginEditingBuildingActor);
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x10B0 / 8, ServerEndEditingBuildingActor);
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x1090 / 8, ServerCreateBuildingActor);
}