#include "pch.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawn.h"

void FortPlayerPawn::ServerHandlePickup(AFortPlayerPawn* FortPlayerPawn, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound)
{
	if (Pickup != NULL)
		Pickup->SetPickupTarget(FortPlayerPawn, InFlyTime / FortPlayerPawn->PickupSpeedMultiplier, InStartDirection);
}

float FortPlayerPawn::GetDefaultHalfHeight(AFortPlayerPawn* FortPlayerPawn)
{
	if (UFortGlobals::IsInSubGame(GWorld, ESubGame::Campaign))
		return 800.f;

	return Originals::GetDefaultHalfHeight(FortPlayerPawn);
}

void FortPlayerPawn::Setup()
{
	Utils::Virtual(AFortPlayerPawn::GetDefaultObj(), 0xD60 / 8, ServerHandlePickup);
	Utils::Virtual(AFortPlayerPawn::GetDefaultObj(), 0x640 / 8, GetDefaultHalfHeight, (void**)&Originals::GetDefaultHalfHeight);
}