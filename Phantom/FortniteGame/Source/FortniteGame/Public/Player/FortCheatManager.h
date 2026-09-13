#pragma once
#include "pch.h"
#include "Utils.h"

class FortCheatManager
{
private:
	static void AddAllScores(UFortCheatManager* Context, FFrame* Stack);
	static void AddBuildingScore(UFortCheatManager* Context, FFrame* Stack);
	static void AddCombatScore(UFortCheatManager* Context, FFrame* Stack);
	static void AddKillFeedMessage(UFortCheatManager* Context, FFrame* Stack);
	static void AddUtilityScore(UFortCheatManager* Context, FFrame* Stack);

	static void AllowRespawn(UFortCheatManager* Context, FFrame* Stack);

	static void ApplyGameplayModifier(UFortCheatManager* Context, FFrame* Stack);
	static void ApplyWeaponAlteration(UFortCheatManager* Context, FFrame* Stack);

	static void AthenaEmote(UFortCheatManager* Context, FFrame* Stack);
	static void AthenaEndlessGame(UFortCheatManager* Context, FFrame* Stack);
	static void AthenaForceVictory(UFortCheatManager* Context, FFrame* Stack);
	static void AthenaGiveScoreTo(UFortCheatManager* Context, FFrame* Stack);

	static void BackpackSetSize(UFortCheatManager* Context, FFrame* Stack);
	static void Badass(UFortCheatManager* Context, FFrame* Stack);

	static void BringDownWall(UFortCheatManager* Context, FFrame* Stack);

	static void BuildFree(UFortCheatManager* Context, FFrame* Stack);
	static void ForceServerShutdown(UFortCheatManager* Context, FFrame* Stack);

	static void GiveConsumable(UFortCheatManager* Context, FFrame* Stack);
	static void GiveGadget(UFortCheatManager* Context, FFrame* Stack);
	static void GiveGadgets(UFortCheatManager* Context, FFrame* Stack);
	static void GivePickaxe(UFortCheatManager* Context, FFrame* Stack);
	static void GiveResources(UFortCheatManager* Context, FFrame* Stack);
	static void GiveSpecificItem(UFortCheatManager* Context, FFrame* Stack);
	static void GiveTrap(UFortCheatManager* Context, FFrame* Stack);
	static void GiveWeapon(UFortCheatManager* Context, FFrame* Stack);
	static void GoFast(UFortCheatManager* Context, FFrame* Stack);
	static void GiveWood(UFortCheatManager* Context, FFrame* Stack);
	static void GiveMetal(UFortCheatManager* Context, FFrame* Stack);
	static void GiveStone(UFortCheatManager* Context, FFrame* Stack);

	static void MassSuicide(UFortCheatManager* Context, FFrame* Stack);

	static void SetHealthPercent(UFortCheatManager* Context, FFrame* Stack);
	static void SetShieldPercent(UFortCheatManager* Context, FFrame* Stack);

	static void TeleportToLocation(UFortCheatManager* Context, FFrame* Stack);

	static void ToggleInfiniteAmmo(UFortCheatManager* Context, FFrame* Stack);
	static void ToggleUnlimitedHealth(UFortCheatManager* Context, FFrame* Stack);
public:
	static void Setup();
};