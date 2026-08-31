#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameState
{
private:
	static void StoreGameSessionID(AFortGameState* GameState, const FString* SessionIDPrefix);
public:
	static void Setup();
};