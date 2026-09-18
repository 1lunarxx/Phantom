#pragma once
#include "pch.h"
#include "Utils.h"

class FortMission
{
	class Originals
	{
	public:
		static inline bool (*InitializeActorsForMissionMatchingItemIdentifyingQuery)(AFortMission* FortMission, AFortMission* Mission, const FGameplayTagQuery* TagQuery, TArray<AActor*>* OutActorsInitialized);
	};

private:
	static bool InitializeActorsForMissionMatchingItemIdentifyingQuery(AFortMission* FortMission, AFortMission* Mission, const FGameplayTagQuery* TagQuery, TArray<AActor*>* OutActorsInitialized);
public:
	static void Setup();
};