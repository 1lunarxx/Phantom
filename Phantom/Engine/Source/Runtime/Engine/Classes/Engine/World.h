#pragma once
#include "pch.h"
#include "Utils.h"

class World
{
public:
	static inline FName NAME_GameNetDriver = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");
private:
	static ENetMode InternalGetNetMode(UWorld* World);
private:
	static bool Listen(UWorld* World, FURL& InURL);
public:
	static void Setup();
};