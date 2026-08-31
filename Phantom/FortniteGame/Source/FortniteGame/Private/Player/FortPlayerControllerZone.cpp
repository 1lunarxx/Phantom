#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerControllerZone.h"

void FortPlayerControllerZone::ServerAcknowledgePossession_Implementation(AFortPlayerControllerZone* FortPlayerControllerZone, APawn* P)
{
	FortPlayerControllerZone->AcknowledgedPawn = P;

	if (AFortPlayerState* PS = Cast<AFortPlayerState>(FortPlayerControllerZone->PlayerState))
	{
		if (FortPlayerControllerZone->MyFortPawn != NULL)
			PS->ApplyCharacterCustomization(FortPlayerControllerZone->MyFortPawn);
	}
}

void FortPlayerControllerZone::Setup()
{
	Utils::Virtual(AFortPlayerControllerZone::GetDefaultObj(), 0x828 / 8, ServerAcknowledgePossession_Implementation);
}