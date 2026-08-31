#include "pch.h"
#include "FortniteGame/Public/Online/FortGameSession.h"

void FortGameSession::PostLogin(AFortGameSession* FortGameSession, AFortPlayerController* NewPlayer)
{
}

void FortGameSession::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x113E210, PostLogin);
}