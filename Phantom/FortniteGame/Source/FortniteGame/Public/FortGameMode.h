#pragma once
#include "pch.h"
#include "Utils.h"

class FortGameMode
{
	class Originals
	{
	public:
		static inline bool (*CanServerTravel)(AFortGameMode* FortGameMode, FString* URL, bool bAbsolute);
	};

private:
	static TSubclassOf<AGameSession>* GetGameSessionClass(AFortGameMode* FortGameMode, TSubclassOf<AGameSession>* Result);
public:
	static void Setup();
};