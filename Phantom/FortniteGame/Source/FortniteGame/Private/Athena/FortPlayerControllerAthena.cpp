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
	if (AFortInventory* WorldInventory = FortPlayerControllerAthena->GetWorldInventory())
	{
		WorldInventory->Inventory.ReplicatedEntries.Free();
		WorldInventory->Inventory.ItemInstances.Free();

		WorldInventory->Inventory.MarkArrayDirty();
	}
}

void FortPlayerControllerAthena::Setup()
{
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x7E8 / 8, ServerRestartPlayer_Implementation);
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x1270 / 8, ServerReturnToMainMenu_Implementation);

	Utils::Rel32(InSDKUtils::GetImageBase() + 0xCBAC01, RemoveAllInventoryItems);
}