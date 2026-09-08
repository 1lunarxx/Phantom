#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerControllerGameplay
{
	class Originals
	{
	public:
		static inline void (*EndGhostMode)(AFortPlayerControllerGameplay* FortPlayerControllerGameplay, FFrame& Stack);
	};

private:
	static void GiveItemToInventoryOwner_StartGhostMode(TScriptInterface<IFortInventoryOwnerInterface> InventoryOwner, UFortWorldItemDefinition* ItemDefinition, int NumberToGive);
	static void EndGhostMode(AFortPlayerControllerGameplay* FortPlayerControllerGameplay, FFrame& Stack);
public:
	static void Setup();
};