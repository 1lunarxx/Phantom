#pragma once
#include "pch.h"
#include "Utils.h"

class FortCheatManager
{
private:
	static void GiveWood(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveMetal(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveStone(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void ToggleInfiniteAmmo(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void BuildFree(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveWeapon(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveBluGlo(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void ForceServerShutdown(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveConsumable(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void TeleportToLocation(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void ToggleUnlimitedHealth(UFortCheatManager* FortCheatManager, FFrame& Stack);
public:
	static void Setup();
};