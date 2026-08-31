#include "pch.h"
#include "FortniteGame/Public/FortGameState.h"

void FortGameState::StoreGameSessionID(AFortGameState* GameState, const FString* SessionIDPrefix)
{
}

void FortGameState::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0xF7E700, StoreGameSessionID);
}