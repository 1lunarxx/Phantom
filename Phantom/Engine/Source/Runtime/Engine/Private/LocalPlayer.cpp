#include "pch.h"
#include "Engine/LocalPlayer.h"

bool LocalPlayer::SpawnPlayActor(ULocalPlayer* LocalPlayer, const FString* URL, FString* OutError, UWorld* InWorld)
{
	return true;
}

void LocalPlayer::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x275E720, SpawnPlayActor);
}