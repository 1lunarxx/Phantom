#include "pch.h"
#include "FortniteGame/Public/Online/FortGameSessionDedicated.h"

void FortGameSessionDedicated::Setup()
{
	Utils::Patch<uint16>(InSDKUtils::GetImageBase() + 0x112C3AC, 0xe990);
}