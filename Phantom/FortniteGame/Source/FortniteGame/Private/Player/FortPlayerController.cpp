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

			if (WorldItem != NULL)
			{
				if (UFortWeaponItemDefinition* WeaponItemDefinition = Cast<UFortWeaponItemDefinition>(WorldItem->ItemEntry.ItemDefinition))
					WeaponItemDefinition->ServerExecute(WorldItem, FortPlayerController);
			}
		}
	}
}

void FortPlayerController::ServerAttemptInventoryDrop_Implementation(AFortPlayerController* FortPlayerController, FGuid* ItemGuid, int Count, bool bTrash)
{
	AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory();

	if (WorldInventory == NULL)
		return;

	IFortInventoryInterface* InventoryInterface = WorldInventory->GetInterfaceAddress<IFortInventoryInterface>();
	IFortInventoryOwnerInterface* InventoryOwnerInterface = FortPlayerController->GetInterfaceAddress<IFortInventoryOwnerInterface>();

	if (InventoryInterface == NULL || InventoryOwnerInterface == NULL)
		return;

	UFortWorldItem* WorldItem = InventoryInterface->GetItem(ItemGuid);

	if (WorldItem == NULL)
		return;

	if (WorldItem->ItemEntry.Count <= 0)
	{
		InventoryOwnerInterface->RemoveInventoryItem(*ItemGuid, Count, true, true);
		return;
	}

	if (InventoryOwnerInterface->RemoveInventoryItem(*ItemGuid, Count, false, false))
	{
		AFortPlayerPawn* FortPlayerPawn = FortPlayerController->GetPlayerPawn();
		
		if (FortPlayerPawn != NULL)
			AFortPickup::SpawnPickup(WorldItem->ItemEntry, FortPlayerPawn->K2_GetActorLocation() + FortPlayerPawn->GetActorForwardVector() * 70.f + FVector(0, 0, 50), Count, EFortPickupSourceTypeFlag::Player, -1, true, true, FortPlayerPawn);
	}
}

void FortPlayerController::ServerCheat_Implementation(AFortPlayerController* FortPlayerController, FString& Msg)
{
	if (FortPlayerController->CheatManager == NULL)
		FortPlayerController->CheatManager = NewObject<UFortCheatManager>(FortPlayerController);

	if (FortPlayerController->CheatManager != NULL)
	{
		FString Result;
		FString* ConsoleCommand = FortPlayerController->ConsoleCommand(&Result, &Msg, true);

		FortPlayerController->ClientMessage(*ConsoleCommand, FName(), 0.f);
	}
}

void FortPlayerController::ServerPlayEmoteItem_Implementation(AFortPlayerController* FortPlayerController, UFortMontageItemDefinitionBase* EmoteAsset)
{
	if (EmoteAsset != NULL)
	{
		FFortAssets::GetAsset(&EmoteAsset->Animation, true); // whats the point of this?
		ServerPlayEmoteItem_Internal(FortPlayerController, EmoteAsset); // not sure the name of this
	}
}

void FortPlayerController::ServerPlayEmoteItem_Internal(AFortPlayerController* FortPlayerController, UFortMontageItemDefinitionBase* EmoteAsset)
{
	AFortPlayerPawn* MyFortPawn = FortPlayerController->MyFortPawn;

	if (MyFortPawn != NULL)
	{
		UAbilitySystemComponent* ASC = MyFortPawn->AbilitySystemComponent;

		if (ASC != NULL)
		{
			if (EmoteAsset != NULL)
			{
				/*if (FortPlayerController->CanPerformNativeAction(SomeTag))*/
				{
					UFortGameData* GameData = UFortGameData::Get();

					if (GameData == NULL)
						return;

					TSubclassOf<UFortGameplayAbility> GameplayAbility;
					FFortAssets::GetSubclassOf(&GameplayAbility, EmoteAsset->IsA(UAthenaSprayItemDefinition::StaticClass()) ? &GameData->SprayGameplayAbility : &GameData->EmoteGameplayAbility, true);

					if (GameplayAbility != NULL)
					{
						UFortGameplayAbility* FortGameplayAbility = Cast<UFortGameplayAbility>(GameplayAbility->DefaultObject);

						if (FortGameplayAbility == NULL)
							return;

						FGameplayAbilitySpec Spec;
						Spec.ConstructAbilitySpec(GameplayAbility->DefaultObject, 1, -1, EmoteAsset);

						FGameplayAbilitySpecHandle Handle;
						ASC->GiveAbilityAndActivateOnce(&Handle, &Spec);
					}
				}
			}
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

void FortPlayerController::ServerCreateBuildingActor(AFortPlayerController* FortPlayerController, FBuildingClassData& BuildingClassData, FVector_NetQuantize10& BuildLoc, FRotator& BuildRot, bool bMirrored, float SyncKey)
{
	if (!FixUpCreateBuildingClassData(FortPlayerController, &BuildingClassData))
		return;

	if (BuildingClassData.BuildingClass != NULL)
	{
		ABuildingSMActor* BuildingSMActor = Cast<ABuildingSMActor>(BuildingClassData.BuildingClass->DefaultObject);

		if (BuildingSMActor == NULL)
			return;

		if (FortPlayerController->CanAffordToPlaceBuildableClass(&BuildingClassData) && FortPlayerController->HasRequiredBuildingLevel(BuildingClassData.PreviousBuildingLevel, BuildingSMActor->ResourceType))
		{
			TArray<ABuildingActor*> ExistingBuildings;
			EFortStructuralGridQueryResults CanPlaceBuildableClassInStructuralGrid = FortPlayerController->CanPlaceBuildableClassInStructuralGrid(BuildingClassData.BuildingClass, &BuildLoc, &BuildRot, bMirrored, &ExistingBuildings);

			if (CanPlaceBuildableClassInStructuralGrid == EFortStructuralGridQueryResults::CanAdd)
			{
				FActorSpawnParameters SpawnParams = FActorSpawnParameters();
				SpawnParams.SpawnCollisionHandlingOverride = 1;

				ABuildingSMActor* NewBuilding = Cast<ABuildingSMActor>(GWorld->SpawnActor(BuildingClassData.BuildingClass, &BuildLoc, &BuildRot, &SpawnParams));

				if (NewBuilding != NULL)
				{
					NewBuilding->CurrentBuildingLevel = BuildingClassData.UpgradeLevel;
					NewBuilding->OnRep_CurrentBuildingLevel();

					NewBuilding->SetMirrored(bMirrored);
					NewBuilding->InitializeKismetSpawnedBuildingActor(NewBuilding, FortPlayerController, true);

					int32 BuildableClassPlacementCost = FortPlayerController->PayBuildableClassPlacementCost(&BuildingClassData);

					if (AFortPlayerControllerAthena* FortPlayerControllerAthena = Cast<AFortPlayerControllerAthena>(FortPlayerController))
					{
						if (GGameState->GamePhase != EAthenaGamePhase::Warmup)
						{
							switch (NewBuilding->ResourceType)
							{
							case EFortResourceType::Wood:
								FortPlayerControllerAthena->MatchReport->MatchStats.Stats[13] += BuildableClassPlacementCost; // GameplayStat.Profile.Match.UsedWood
								break;
							case EFortResourceType::Stone:
								FortPlayerControllerAthena->MatchReport->MatchStats.Stats[14] += BuildableClassPlacementCost; // GameplayStat.Profile.Match.UsedStone
								break;
							case EFortResourceType::Metal:
								FortPlayerControllerAthena->MatchReport->MatchStats.Stats[15] += BuildableClassPlacementCost; // GameplayStat.Profile.Match.UsedMetal
								break;
							}
						}
					}

					UFortAnalytics::FireEvent_BuildingAction(FortPlayerController, L"Create", BuildingSMActor, 0);

					if (GGameMode != NULL)
						GGameMode->ScoreBuildingConstruction(FortPlayerController, NewBuilding);

					if (AFortPlayerPawn* MyFortPawn = FortPlayerController->MyFortPawn)
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

void FortPlayerController::ServerBeginEditingBuildingActor(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToEdit)
{
	if (BuildingActorToEdit != NULL && FortPlayerController->MyFortPawn != NULL)
	{
		AFortPlayerStateZone* PlayerState = Cast<AFortPlayerStateZone>(FortPlayerController->PlayerState);

		if (PlayerState != NULL)
		{
			BuildingActorToEdit->SetEditingPlayer(PlayerState);

			UFortEditToolItemDefinition* EditToolItem = UFortGameData::Get()->EditToolItem.LoadSynchronous();
			UFortWorldItem* WorldItem = FortPlayerController->WorldInventory->FindExistingItemForDefinition(EditToolItem);

			if (EditToolItem != NULL && WorldItem != NULL && EditToolItem->ServerExecute(WorldItem, FortPlayerController))
			{
				AFortWeap_EditingTool* EditingTool = Cast<AFortWeap_EditingTool>(FortPlayerController->MyFortPawn->CurrentWeapon);

				if (EditingTool != NULL)
					EditingTool->SetEditActor(BuildingActorToEdit);
			}
		}
	}
}

void FortPlayerController::ServerEditBuildingActor(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, uint8 RotationIterations, bool bMirrored)
{
	if (BuildingActorToEdit != NULL && BuildingActorToEdit->EditingPlayer == FortPlayerController->PlayerState && !BuildingActorToEdit->bDestroyed)
	{
		BuildingActorToEdit->SetEditingPlayer(NULL);
		BuildingActorToEdit->ReplaceBuildingActor(EBuildingReplacementType::BRT_Edited, NewBuildingClass, BuildingActorToEdit->GetCurrentBuildingLevel(), RotationIterations, bMirrored, FortPlayerController);

		UFortAnalytics::FireEvent_BuildingAction(FortPlayerController, L"Edit", BuildingActorToEdit, 0);

		if (GGameMode != NULL)
			GGameMode->ScoreBuildingEdit(FortPlayerController, BuildingActorToEdit);
	}
}

void FortPlayerController::ServerEndEditingBuildingActor(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToEdit)
{
	if (BuildingActorToEdit != NULL && FortPlayerController->MyFortPawn != NULL && BuildingActorToEdit->EditingPlayer == FortPlayerController->PlayerState && !BuildingActorToEdit->bDestroyed)
	{
		BuildingActorToEdit->SetEditingPlayer(NULL);

		UFortEditToolItemDefinition* EditToolItem = UFortGameData::Get()->EditToolItem.LoadSynchronous();
		UFortWorldItem* WorldItem = FortPlayerController->WorldInventory->FindExistingItemForDefinition(EditToolItem);

		if (EditToolItem != NULL && WorldItem != NULL && EditToolItem->ServerExecute(WorldItem, FortPlayerController))
		{
			AFortWeap_EditingTool* EditingTool = Cast<AFortWeap_EditingTool>(FortPlayerController->MyFortPawn->CurrentWeapon);

			if (EditingTool != NULL)
				EditingTool->SetEditActor(NULL);
		}
	}
}

void FortPlayerController::ServerRepairBuildingActor(AFortPlayerController* FortPlayerController, ABuildingSMActor* BuildingActorToRepair)
{
	if (BuildingActorToRepair == NULL)
		return;

	int32 CostToRepair = FortPlayerController->PayBuildingRepairCost(BuildingActorToRepair);

	BuildingActorToRepair->RepairBuilding(FortPlayerController, CostToRepair);
}

void FortPlayerController::DropItemsOnPawnDestruction(AFortPlayerController* FortPlayerController, AFortPlayerController::EPawnDestructionReason DestructionReason, const FGameplayTagContainer* ContextualTags, AFortPawn* DestructionPawn)
{
	if (DestructionPawn == NULL)
		return;

	if (AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory())
	{
		for (UFortWorldItem* WorldItem : WorldInventory->Inventory.ItemInstances)
		{
			if (WorldItem->CanBeDropped())
				AFortPickup::SpawnPickup(WorldItem->ItemEntry, DestructionPawn->K2_GetActorLocation(), WorldItem->ItemEntry.Count, EFortPickupSourceTypeFlag::Player, -1);
		}
	}
}

void FortPlayerController::Setup()
{
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0xFA0 / 8, ServerExecuteInventoryItem_Implementation);
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0x1030 / 8, ServerAttemptInventoryDrop_Implementation);
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0xDC0 / 8, ServerPlayEmoteItem_Implementation);
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0xDB0 / 8, ServerCheat_Implementation);

	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0x10A0 / 8, ServerEditBuildingActor);
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0x10C0 / 8, ServerBeginEditingBuildingActor);
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0x10B0 / 8, ServerEndEditingBuildingActor);
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0x1090 / 8, ServerCreateBuildingActor);

	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0x1070 / 8, ServerRepairBuildingActor);
	Utils::Virtual(AFortPlayerController::GetDefaultObj(), 0x1900 / 8, DropItemsOnPawnDestruction);
}