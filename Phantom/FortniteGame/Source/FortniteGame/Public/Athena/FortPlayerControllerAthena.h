#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerControllerAthena
{
private:
	static void ServerRestartPlayer_Implementation(AFortPlayerControllerAthena* FortPlayerControllerAthena);
public:
	static void Setup();
};