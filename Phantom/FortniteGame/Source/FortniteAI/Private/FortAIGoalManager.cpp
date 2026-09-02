#include "pch.h"
#include "FortniteAI/Public/FortAIGoalManager.h"

AFortAIGoalManager* FortAIGoalManager::GetCurrent(UWorld* World)
{
	return GGameMode->AIGoalManager;
}

void FortAIGoalManager::Setup()
{
	TArray<uintptr_t> GetCurrent_NullSubs;
}