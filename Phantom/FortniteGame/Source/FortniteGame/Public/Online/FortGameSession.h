#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameSession
{
private:
	static void PostLogin(AFortGameSession* FortGameSession, AFortPlayerController* NewPlayer);
public:
	static void Setup();
};