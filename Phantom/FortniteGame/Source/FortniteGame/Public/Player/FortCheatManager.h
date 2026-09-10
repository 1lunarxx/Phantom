#pragma once
#include "pch.h"
#include "Utils.h"

class FortCheatManager
{
private:
	static void AddAllScores(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void AddBuildingScore(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void AddCombatScore(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void AddKillFeedMessage(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void AddUtilityScore(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void AllowRespawn(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void ApplyGameplayModifier(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void ApplyWeaponAlteration(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void AthenaEmote(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void AthenaEndlessGame(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void AthenaForceVictory(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void AthenaGiveScoreTo(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void BackpackSetSize(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void Badass(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void BringDownWall(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void BuildFree(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void ForceServerShutdown(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void GetWorldLevel(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void GiveConsumable(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveGadget(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveGadgets(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GivePickaxe(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveResources(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveSpecificItem(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveTrap(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveWeapon(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GoFast(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveWood(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveMetal(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void GiveStone(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void MassSuicide(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void SetHealthPercent(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void SetShieldPercent(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void TeleportToLocation(UFortCheatManager* FortCheatManager, FFrame& Stack);

	static void ToggleInfiniteAmmo(UFortCheatManager* FortCheatManager, FFrame& Stack);
	static void ToggleUnlimitedHealth(UFortCheatManager* FortCheatManager, FFrame& Stack);
public:
	static void Setup();
};