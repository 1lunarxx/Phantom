#include "pch.h"
#include "FortniteGame/Public/Athena/FortGameStateAthena.h"

void FortGameStateAthena::Setup()
{
	Utils::Patch<uint8_t>(InSDKUtils::GetImageBase() + 0xC96B25 + 1, 0x85); // CanUpdateGamePhaseStep
}