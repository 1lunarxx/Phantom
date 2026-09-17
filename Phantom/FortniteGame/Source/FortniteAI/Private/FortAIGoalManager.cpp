#include "pch.h"
#include "FortniteAI/Public/FortAIGoalManager.h"

AFortAIGoalManager* FortAIGoalManager::GetCurrent(UWorld* World)
{
	if (AFortGameModeZone* FortGameModeZone = Cast<AFortGameModeZone>(World->AuthorityGameMode))
	{
		return FortGameModeZone->AIGoalManager;
	}

	return NULL;
}

void FortAIGoalManager::Setup()
{
	TArray<uintptr_t> GetCurrent_NullSubs;

	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xAE1E69);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xB02AFD);

	for (uintptr_t GetCurrent_NullSub : GetCurrent_NullSubs)
		Utils::Rel32(GetCurrent_NullSub, GetCurrent);
}