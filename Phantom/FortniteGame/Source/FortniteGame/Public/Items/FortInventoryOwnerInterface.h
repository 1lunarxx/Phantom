#pragma once
#include "pch.h"
#include "Utils.h"

class FortInventoryOwnerInterface
{
private:
	static bool RemoveInventoryItem(IFortInventoryOwnerInterface* FortInventoryOwnerInterface, FGuid& ItemGuid, int32 Count, bool bForceRemoval, bool bForcePersistWhenEmpty);
public:
	static void Setup();
};