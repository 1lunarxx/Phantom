#pragma once
#include "pch.h"
#include "Utils.h"

class FortPickup
{
	class Originals
	{
	public:
		static inline void (*GivePickupTo)(AFortPickup* FortPickup, IFortInventoryOwnerInterface* InventoryOwner, bool DestoryAfterPickup);
	};

private:
	static void GivePickupTo(AFortPickup* FortPickup, IFortInventoryOwnerInterface* InventoryOwner, bool DestoryAfterPickup);
public:
	static void Setup();
};