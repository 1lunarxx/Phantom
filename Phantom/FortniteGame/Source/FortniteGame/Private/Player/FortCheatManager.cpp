#include "pch.h"
#include "FortniteGame/Public/Player/FortCheatManager.h"
#include "Core/Public/GenericPlatform/GenericPlatformMisc.h"

void UFortCheatManager::AddScoreStat(unsigned int ScoreStat, unsigned int Amount)
{
}

void FortCheatManager::AddAllScores(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Amount;

	Stack.StepCompiledIn(&Amount);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	FortCheatManager->AddScoreStat(1u, Amount);
	FortCheatManager->AddScoreStat(9u, Amount);
	FortCheatManager->AddScoreStat(0x18u, Amount);
}

void FortCheatManager::AddBuildingScore(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Amount;

	Stack.StepCompiledIn(&Amount);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	FortCheatManager->AddScoreStat(9, Amount);
}

void FortCheatManager::AddCombatScore(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Amount;

	Stack.StepCompiledIn(&Amount);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	FortCheatManager->AddScoreStat(1u, Amount);
}

void FortCheatManager::AddKillFeedMessage(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;
}

void FortCheatManager::AddUtilityScore(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Amount;

	Stack.StepCompiledIn(&Amount);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	FortCheatManager->AddScoreStat(0x18u, Amount);
}

void FortCheatManager::AllowRespawn(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	UFortPlaylistAthena* FortPlaylistAthena = GGameState->GetCurrentPlaylistData();

	if (FortPlaylistAthena != NULL)
		FortPlaylistAthena->RespawnType = EAthenaRespawnType::InfiniteRespawn;
}

void FortCheatManager::ApplyGameplayModifier(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	FString ModifierName;

	Stack.StepCompiledIn(&ModifierName);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController != NULL)
		FortPlayerController->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
}

void FortCheatManager::ApplyWeaponAlteration(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	FString AlterationName;

	Stack.StepCompiledIn(&AlterationName);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController != NULL)
		FortPlayerController->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
}

// this should be something with UFortAssetManager::GetItemTypeData but i cba to do that
void FortCheatManager::AthenaEmote(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	FString EmoteName;

	Stack.StepCompiledIn(&EmoteName);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UAthenaDanceItemDefinition* DanceItemDefinition = Utils::StaticFindObject<UAthenaDanceItemDefinition>(EmoteName.CStr(), ANY_PACKAGE);

	if (DanceItemDefinition == NULL)
		return;

	FortPlayerController->ServerPlayEmoteItem(DanceItemDefinition);
}

void FortCheatManager::AthenaEndlessGame(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController != NULL)
		FortPlayerController->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
}

void FortCheatManager::AthenaForceVictory(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	bool bVictory;

	Stack.StepCompiledIn(&bVictory);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	AFortPlayerStateAthena* FortPlayerState = Cast<AFortPlayerStateAthena>(FortPlayerController->PlayerState);

	if (FortPlayerState == NULL)
		return;

	GGameMode->StartEndGamePhaseTeam((int)FortPlayerState->TeamIndex, FortPlayerState, 1, FortPlayerController->Pawn, NULL, EDeathCause::WonMatch);
}

void FortCheatManager::AthenaGiveScoreTo(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 PlayerStateIdx;
	int32 Score;

	Stack.StepCompiledIn(&PlayerStateIdx);
	Stack.StepCompiledIn(&Score);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortPlaylistAthena* CurrentPlaylistData = GGameState->GetCurrentPlaylistData();

	if (CurrentPlaylistData != NULL)
	{
		if (PlayerStateIdx < GGameState->PlayerArray.Num())
		{
			bool bValidIndex = false;

			if (PlayerStateIdx < GGameState->PlayerArray.Num())
				bValidIndex = PlayerStateIdx >= 0;

			if (!bValidIndex)
				return;

			AFortPlayerStateAthena* FortPlayerStateAthena = Cast<AFortPlayerStateAthena>(FortPlayerController->PlayerState);

			if (FortPlayerStateAthena != NULL)
			{
				if (CurrentPlaylistData->ScoringData.ScoreDataList.Num() > 0)
				{
				}
			}
		}
	}

	FortPlayerController->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
}

void FortCheatManager::BackpackSetSize(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Size;

	Stack.StepCompiledIn(&Size);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	FortPlayerController->OverriddenBackpackSize = Size;
}

void FortCheatManager::Badass(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	FortCheatManager->SetShieldPercent(1000);
	FortCheatManager->SetHealthPercent(1000);
}

void FortCheatManager::BringDownWall(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	FortPlayerController->ClientMessage(L"Cheat Command not implemented!", FName(), 0.f);
}

void FortCheatManager::BuildFree(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController != NULL)
		FortPlayerController->bBuildFree = true;
}

void FortCheatManager::ForceServerShutdown(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 ExitCode;

	Stack.StepCompiledIn(&ExitCode);
	Stack.IncrementCode();

	for (AFortPlayerControllerAthena* FortPlayerController : GGameMode->AlivePlayers)
	{
		FortPlayerController->ServerReturnToMainMenu();
	}

	if (!ExitCode)
		FGenericPlatformMisc::RequestExit(false);
}

void FortCheatManager::GetWorldLevel(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	FString Message = (L"World Level: " + std::to_wstring(GGameState->WorldLevel)).c_str();

	FortPlayerController->ClientMessage(Message, FName(), 0.f);
}

void FortCheatManager::GiveConsumable(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	FString ConsumableName;
	int32 RequestedLevel;
	int32 Count;

	Stack.StepCompiledIn(&ConsumableName);
	Stack.StepCompiledIn(&RequestedLevel);
	Stack.StepCompiledIn(&Count);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortItemDefinition* FortItemDefinition = Utils::StaticFindObject<UFortItemDefinition>(ConsumableName.CStr(), ANY_PACKAGE);

	if (FortItemDefinition == NULL)
	{
		FortPlayerController->ClientMessage(L"Couldn't find ItemDefinition from the request ConsumableName!", FName(), 0.f);
		return;
	}

	FortPlayerController->WorldInventory->AddItem(FortItemDefinition, Count);
}

void FortCheatManager::GiveGadget(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	FString GadgetName;
	int32 ItemLevel;

	Stack.StepCompiledIn(&GadgetName);
	Stack.StepCompiledIn(&ItemLevel);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortGadgetItemDefinition* FortGadgetItemDefinition = Utils::StaticFindObject<UFortGadgetItemDefinition>(GadgetName.CStr(), ANY_PACKAGE);

	if (FortGadgetItemDefinition == NULL)
	{
		FortPlayerController->ClientMessage(L"Couldn't find FortGadgetItemDefinition from the request GadgetName!", FName(), 0.f);
		return;
	}

	FortPlayerController->WorldInventory->AddItem(FortGadgetItemDefinition, 1);
}

void FortCheatManager::GiveGadgets(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	FString GadgetName;
	int32 Quantity;
	int32 ItemLevel;

	Stack.StepCompiledIn(&GadgetName);
	Stack.StepCompiledIn(&Quantity);
	Stack.StepCompiledIn(&ItemLevel);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortGadgetItemDefinition* FortGadgetItemDefinition = Utils::StaticFindObject<UFortGadgetItemDefinition>(GadgetName.CStr(), ANY_PACKAGE);

	if (FortGadgetItemDefinition == NULL)
	{
		FortPlayerController->ClientMessage(L"Couldn't find FortGadgetItemDefinition from the request GadgetName!", FName(), 0.f);
		return;
	}

	FortPlayerController->WorldInventory->AddItem(FortGadgetItemDefinition, Quantity);
}

void FortCheatManager::GivePickaxe(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	FString PickName;

	Stack.StepCompiledIn(&PickName);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UAthenaPickaxeItemDefinition* PickaxeItemDefinition = Utils::StaticFindObject<UAthenaPickaxeItemDefinition>(PickName.CStr(), ANY_PACKAGE);

	if (PickaxeItemDefinition == NULL)
	{
		FortPlayerController->ClientMessage(L"Couldn't find PickaxeItemDefinition from the request PickName!", FName(), 0.f);
		return;
	}

	FortPlayerController->WorldInventory->AddItem(PickaxeItemDefinition, 1);
}

void FortCheatManager::GiveResources(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Amount;

	Stack.StepCompiledIn(&Amount);
	Stack.IncrementCode();

	FortCheatManager->GiveMetal(Amount);
	FortCheatManager->GiveWood(Amount);
	FortCheatManager->GiveStone(Amount);
}

void FortCheatManager::GiveSpecificItem(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	FString ItemPath;
	int32 ItemAmount;
	int32 ItemLevel;

	Stack.StepCompiledIn(&ItemPath);
	Stack.StepCompiledIn(&ItemAmount);
	Stack.StepCompiledIn(&ItemLevel);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortItemDefinition* FortItemDefinition = Utils::StaticFindObject<UFortItemDefinition>(ItemPath.CStr());

	if (FortItemDefinition == NULL)
	{
		FortPlayerController->ClientMessage(L"Couldn't find FortItemDefinition from the request ItemPath!", FName(), 0.f);
		return;
	}

	FortPlayerController->WorldInventory->AddItem(FortItemDefinition, ItemAmount);
}

void FortCheatManager::GiveTrap(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	FString TrapName;
	int32 RequestedLevel;
	int32 NumTrapsToGive;

	Stack.StepCompiledIn(&TrapName);
	Stack.StepCompiledIn(&RequestedLevel);
	Stack.StepCompiledIn(&NumTrapsToGive);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortTrapItemDefinition* FortTrapItemDefinition = Utils::StaticFindObject<UFortTrapItemDefinition>(TrapName.CStr(), ANY_PACKAGE);

	if (FortTrapItemDefinition == NULL)
	{
		FortPlayerController->ClientMessage(L"Couldn't find FortTrapItemDefinition from the request TrapName!", FName(), 0.f);
		return;
	}

	FortPlayerController->WorldInventory->AddItem(FortTrapItemDefinition, NumTrapsToGive);
}

void FortCheatManager::GiveWeapon(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	FString WeaponName;
	int32 RequestedLevel;
	int32 Count;

	Stack.StepCompiledIn(&WeaponName);
	Stack.StepCompiledIn(&RequestedLevel);
	Stack.StepCompiledIn(&Count);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortWeaponItemDefinition* FortWeaponItemDefinition = Utils::StaticFindObject<UFortWeaponItemDefinition>(WeaponName.CStr(), ANY_PACKAGE);

	if (FortWeaponItemDefinition == NULL)
	{
		FortPlayerController->ClientMessage(L"Couldn't find weapon from the request WeaponName!", FName(), 0.f);
		return;
	}

	FortPlayerController->WorldInventory->AddItem(FortWeaponItemDefinition, Count);
}

// bad way to do it but why not

void FortCheatManager::GoFast(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	float NewSpeed;

	Stack.StepCompiledIn(&NewSpeed);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	int32 Speed = (int)NewSpeed;
	FString Message = (L"demospeed " + std::to_wstring(Speed)).c_str();

	UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), Message, FortPlayerController);
}

void FortCheatManager::GiveWood(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Amount;

	Stack.StepCompiledIn(&Amount);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortResourceItemDefinition* WoodItemDefinition = UFortGameData::Get()->WoodItemDefinition.LoadSynchronous();

	if (WoodItemDefinition != NULL)
		FortPlayerController->WorldInventory->AddItemStack(WoodItemDefinition, Amount);
}

void FortCheatManager::GiveMetal(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Amount;

	Stack.StepCompiledIn(&Amount);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortResourceItemDefinition* MetalItemDefinition = UFortGameData::Get()->MetalItemDefinition.LoadSynchronous();

	if (MetalItemDefinition != NULL)
		FortPlayerController->WorldInventory->AddItemStack(MetalItemDefinition, Amount);
}

void FortCheatManager::GiveStone(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Amount;

	Stack.StepCompiledIn(&Amount);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortResourceItemDefinition* StoneItemDefinition = UFortGameData::Get()->StoneItemDefinition.LoadSynchronous();

	if (StoneItemDefinition != NULL)
		FortPlayerController->WorldInventory->AddItemStack(StoneItemDefinition, Amount);
}

void FortCheatManager::MassSuicide(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	for (AFortPlayerControllerAthena* FortPlayerController : GGameMode->AlivePlayers)
	{
		FortPlayerController->Suicide();
	}
}

void FortCheatManager::SetHealthPercent(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	float Percent;

	Stack.StepCompiledIn(&Percent);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	if (AFortPlayerPawn* FortPlayerPawn = FortPlayerController->GetPlayerPawn())
	{
		if (Percent > FortPlayerPawn->GetMaxHealth())
			return;

		FortPlayerPawn->SetHealth(Percent);
	}
}

void FortCheatManager::SetShieldPercent(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	float Percent;

	Stack.StepCompiledIn(&Percent);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	if (AFortPlayerPawn* FortPlayerPawn = FortPlayerController->GetPlayerPawn())
	{
		if (Percent > FortPlayerPawn->GetMaxShield())
			return;

		FortPlayerPawn->SetShield(Percent);
	}
}

void FortCheatManager::TeleportToLocation(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	float X;
	float Y;
	float Z;

	Stack.StepCompiledIn(&X);
	Stack.StepCompiledIn(&Y);
	Stack.StepCompiledIn(&Z);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	if (AFortPlayerPawn* FortPlayerPawn = FortPlayerController->GetPlayerPawn())
	{
		FortPlayerPawn->K2_TeleportTo(FVector(X, Y, Z), FRotator());
	}
}

void FortCheatManager::ToggleInfiniteAmmo(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController != NULL)
		FortPlayerController->bInfiniteAmmo = true;
}

void FortCheatManager::ToggleUnlimitedHealth(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	if (AFortPlayerPawn* FortPlayerPawn = FortPlayerController->GetPlayerPawn())
	{
		FortPlayerPawn->SetHealth(99999999999999); // idk what to do here tbh
	}
}

void FortCheatManager::Setup()
{
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.AddAllScores"), AddAllScores);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.AddBuildingScore"), AddBuildingScore);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.AddCombatScore"), AddCombatScore);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.AddKillFeedMessage"), AddKillFeedMessage);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.AddUtilityScore"), AddUtilityScore);

	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.AllowRespawn"), AllowRespawn);

	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.ApplyGameplayModifier"), ApplyGameplayModifier);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.ApplyWeaponAlteration"), ApplyWeaponAlteration);

	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.AthenaEmote"), AthenaEmote);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.AthenaEndlessGame"), AthenaEndlessGame);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.AthenaForceVictory"), AthenaForceVictory);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.AthenaGiveScoreTo"), AthenaGiveScoreTo);

	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.BackpackSetSize"), BackpackSetSize);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.Badass"), Badass);

	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.BuildFree"), BuildFree);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.ForceServerShutdown"), ForceServerShutdown);

	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveConsumable"), GiveConsumable);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveGadget"), GiveGadget);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveGadgets"), GiveGadgets);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GivePickaxe"), GivePickaxe);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveResources"), GiveResources);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveSpecificItem"), GiveSpecificItem);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveTrap"), GiveTrap);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveWeapon"), GiveWeapon);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveWood"), GiveWood);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GoFast"), GoFast);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveMetal"), GiveMetal);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveStone"), GiveStone);

	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GoFast"), GoFast);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.MassSuicide"), MassSuicide);

	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.SetHealthPercent"), SetHealthPercent);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.SetShieldPercent"), SetShieldPercent);

	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.TeleportToLocation"), TeleportToLocation);

	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.ToggleInfiniteAmmo"), ToggleInfiniteAmmo);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.ToggleUnlimitedHealth"), ToggleUnlimitedHealth);
}