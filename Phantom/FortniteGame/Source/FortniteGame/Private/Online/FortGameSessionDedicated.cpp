#include "pch.h"
#include "FortniteGame/Public/Online/FortGameSessionDedicated.h"

void FortGameSessionDedicated::Setup()
{
	Utils::Patch<uint8_t>(InSDKUtils::GetImageBase() + 0x112C369 + 1, 0x85); // Dedicated Server Ready!
}