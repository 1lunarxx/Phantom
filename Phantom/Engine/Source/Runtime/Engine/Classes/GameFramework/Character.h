#pragma once
#include "pch.h"
#include "Utils.h"

class Character
{
private:
	static void ClientCheatFly_Implementation(ACharacter* Character);
	static void ClientCheatGhost_Implementation(ACharacter* Character);
	static void ClientCheatWalk_Implementation(ACharacter* Character);
public:
	static void Setup();
};