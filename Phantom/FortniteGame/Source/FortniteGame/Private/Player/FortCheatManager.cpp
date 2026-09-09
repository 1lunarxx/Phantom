#include "pch.h"
#include "FortniteGame/Public/Player/FortCheatManager.h"
#include "Core/Public/GenericPlatform/GenericPlatformMisc.h"

void FortCheatManager::GiveWood(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Amount;

	Stack.StepCompiledIn(&Amount);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortResourceItemDefinition* WoodItemDefinition = GetGameData()->WoodItemDefinition.LoadSynchronous();

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

	UFortResourceItemDefinition* MetalItemDefinition = GetGameData()->MetalItemDefinition.LoadSynchronous();

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

	UFortResourceItemDefinition* StoneItemDefinition = GetGameData()->StoneItemDefinition.LoadSynchronous();

	if (StoneItemDefinition != NULL)
		FortPlayerController->WorldInventory->AddItemStack(StoneItemDefinition, Amount);
}

void FortCheatManager::ToggleInfiniteAmmo(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController != NULL)
		FortPlayerController->bInfiniteAmmo	= true;
}

void FortCheatManager::BuildFree(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController != NULL)
		FortPlayerController->bBuildFree = true;
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

void FortCheatManager::GiveBluGlo(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 Amount;

	Stack.StepCompiledIn(&Amount);
	Stack.IncrementCode();

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(FortCheatManager->Outer);

	if (FortPlayerController == NULL)
		return;

	UFortNeverPersistItemDefinition* BluGloItemDefinition = GetGameData()->BluGloItemDefinition.LoadSynchronous();

	if (BluGloItemDefinition != NULL)
		FortPlayerController->WorldInventory->AddItemStack(BluGloItemDefinition, Amount);
}

void FortCheatManager::ForceServerShutdown(UFortCheatManager* FortCheatManager, FFrame& Stack)
{
	int32 ExitCode;

	Stack.StepCompiledIn(&ExitCode);
	Stack.IncrementCode();

/*	for (UNetConnection* Connection : GWorld->NetDriver->ClientConnections)
	{
		Connection->Close();
	}*/

	for (AFortPlayerControllerAthena* FortPlayerController : GGameMode->AlivePlayers)
	{
		FortPlayerController->ServerReturnToMainMenu();
	}

	if (!ExitCode)
		FGenericPlatformMisc::RequestExit(false);
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
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveWood"), GiveWood);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveMetal"), GiveMetal);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveStone"), GiveStone);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.ToggleInfiniteAmmo"), ToggleInfiniteAmmo);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.BuildFree"), BuildFree);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveWeapon"), GiveWeapon);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveBluGlo"), GiveBluGlo);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.ForceServerShutdown"), ForceServerShutdown);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.GiveConsumable"), GiveConsumable);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.TeleportToLocation"), TeleportToLocation);
	Utils::Exec(TEXT("/Script/FortniteGame.FortCheatManager.ToggleUnlimitedHealth"), ToggleUnlimitedHealth);
}