#include "pch.h"
#include "FortniteGame/Public/Athena/FortPlayerControllerAthena.h"

void FortPlayerControllerAthena::ServerRestartPlayer_Implementation(AFortPlayerControllerAthena* FortPlayerControllerAthena)
{
	static void(*ServerRestartPlayer)(AFortPlayerControllerZone*) = decltype(ServerRestartPlayer)(AFortPlayerControllerZone::GetDefaultObj()->VTable[0x7E8 / 8]);
	ServerRestartPlayer(FortPlayerControllerAthena);
}

void FortPlayerControllerAthena::ServerReturnToMainMenu_Implementation(AFortPlayerControllerAthena* FortPlayerControllerAthena)
{
	static void(*ServerReturnToMainMenu)(AFortPlayerControllerZone*) = decltype(ServerReturnToMainMenu)(AFortPlayerControllerZone::GetDefaultObj()->VTable[0x1270 / 8]);
	ServerReturnToMainMenu(FortPlayerControllerAthena);
}

void FortPlayerControllerAthena::RemoveAllInventoryItems(AFortPlayerControllerAthena* FortPlayerControllerAthena)
{
	AFortInventory* WorldInventory = FortPlayerControllerAthena->GetWorldInventory();

	if (WorldInventory == NULL)
		return;

	WorldInventory->Inventory.ReplicatedEntries.Free();
	WorldInventory->Inventory.ItemInstances.Free();
	WorldInventory->Inventory.MarkArrayDirty();
}

void FortPlayerControllerAthena::OnPawnDied(AFortPlayerControllerAthena* FortPlayerControllerAthena, float Damage, const FGameplayTagContainer* InTags, const FGameplayEffectContextHandle* EffectContext, AController* EventInstigator, AActor* DamageCauser)
{
	Originals::OnPawnDied(FortPlayerControllerAthena, Damage, InTags, EffectContext, EventInstigator, DamageCauser);

	if (FortPlayerControllerAthena == NULL || InTags == NULL)
		return;

	if (EventInstigator == NULL)
		EventInstigator = FortPlayerControllerAthena;

	AFortPlayerPawnAthena* FortPlayerPawnAthena = Cast<AFortPlayerPawnAthena>(FortPlayerControllerAthena->GetPlayerPawn());

	if (FortPlayerPawnAthena == NULL)
		return;

	FortPlayerPawnAthena->InitializeDeathInfoOnPawnDeath(InTags, EventInstigator, FortPlayerPawnAthena->bWasDBNOOnDeath ? EventInstigator : NULL);
}

void FortPlayerControllerAthena::Setup()
{
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x7E8 / 8, ServerRestartPlayer_Implementation);
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x1270 / 8, ServerReturnToMainMenu_Implementation);

	Utils::Hook(InSDKUtils::GetImageBase() + 0xCE1AB0, OnPawnDied, (void**)&Originals::OnPawnDied);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xCBAC01, RemoveAllInventoryItems);
}