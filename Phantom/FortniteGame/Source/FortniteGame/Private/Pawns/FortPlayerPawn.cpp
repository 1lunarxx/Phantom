#include "pch.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawn.h"

void FortPlayerPawn::ServerHandlePickup(AFortPlayerPawn* FortPlayerPawn, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound)
{
	if (Pickup != NULL)
		Pickup->SetPickupTarget(FortPlayerPawn, InFlyTime / FortPlayerPawn->PickupSpeedMultiplier, InStartDirection);
}

void FortPlayerPawn::Setup()
{
	Utils::Virtual(AFortPlayerPawn::GetDefaultObj(), 0xD60 / 8, ServerHandlePickup);
}