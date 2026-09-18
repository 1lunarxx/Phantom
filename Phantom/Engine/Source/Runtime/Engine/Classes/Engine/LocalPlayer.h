#pragma once
#include "pch.h"
#include "Utils.h"

class LocalPlayer
{
private:
	static bool SpawnPlayActor(ULocalPlayer* LocalPlayer, const FString* URL, FString* OutError, UWorld* InWorld);
public:
	static void Setup();
};