#pragma once
#include "pch.h"
#include "Utils.h"

class World
{
private:
	static ENetMode InternalGetNetMode(UWorld* World);
private:
	static bool Listen(UWorld* World, FURL& InURL);
public:
	static void Setup();
};