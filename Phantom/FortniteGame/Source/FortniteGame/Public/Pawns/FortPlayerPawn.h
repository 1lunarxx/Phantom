#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerPawn
{
	class Originals
	{
	public:
		static inline float(*GetDefaultHalfHeight)(AFortPlayerPawn* FortPlayerPawn);
	};

private:
	static void ServerHandlePickup(AFortPlayerPawn* FortPlayerPawn, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound);
	static float GetDefaultHalfHeight(AFortPlayerPawn* FortPlayerPawn);
public:
	static void Setup();
};