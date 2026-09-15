#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerPawn
{
private:
	static void ServerHandlePickup(AFortPlayerPawn* FortPlayerPawn, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound);
public:
	static void Setup();
};