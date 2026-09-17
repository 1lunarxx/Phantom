#include "pch.h"
#include "FortniteAI/Public/FortAIGoalManager.h"

AFortAIGoalManager* FortAIGoalManager::GetCurrent(UObject* WorldContextObject)
{
	if (UWorld* World = Cast<UWorld>(WorldContextObject))
	{
		if (AFortGameModeZone* FortGameModeZone = Cast<AFortGameModeZone>(World->AuthorityGameMode))
		{
			return FortGameModeZone->AIGoalManager;
		}
	}

	UWorld* WorldFromContextObject = GEngine->GetWorldFromContextObject(WorldContextObject, LogAndReturnNull);

	if (WorldFromContextObject != NULL)
	{
		if (AFortGameModeZone* FortGameModeZone = Cast<AFortGameModeZone>(WorldFromContextObject->AuthorityGameMode))
		{
			return FortGameModeZone->AIGoalManager;
		}
	}

	return NULL;
}

void FortAIGoalManager::Setup()
{
	TArray<uintptr_t> GetCurrent_WorldContext_NullSubs;

	GetCurrent_WorldContext_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10B4A81);
	GetCurrent_WorldContext_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10B4CB1);
	GetCurrent_WorldContext_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10B8F65);
	GetCurrent_WorldContext_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10BAF66);
	GetCurrent_WorldContext_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D1401);
	GetCurrent_WorldContext_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D1481);
	GetCurrent_WorldContext_NullSubs.Add(InSDKUtils::GetImageBase() + 0xB36840);
	GetCurrent_WorldContext_NullSubs.Add(InSDKUtils::GetImageBase() + 0xB4C2FA);
	GetCurrent_WorldContext_NullSubs.Add(InSDKUtils::GetImageBase() + 0xAE1E69);
	GetCurrent_WorldContext_NullSubs.Add(InSDKUtils::GetImageBase() + 0xB02AFD);

	for (uintptr_t GetCurrent_NullSub : GetCurrent_WorldContext_NullSubs)
		Utils::Rel32(GetCurrent_NullSub, GetCurrent);
}