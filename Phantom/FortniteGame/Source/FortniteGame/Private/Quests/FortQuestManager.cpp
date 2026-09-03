#include "pch.h"
#include "FortniteGame/Public/Quests/FortQuestManager.h"

void FortQuestManager::SendStatEventWithTags(UFortQuestManager* QuestManager, EFortQuestObjectiveStatEvent Type, UObject* TargetObject, FGameplayTagContainer* TargetTags, FGameplayTagContainer* SourceTags, FGameplayTagContainer* ContextTags, int Count)
{
}

void FortQuestManager::Setup()
{
	TArray<uintptr_t> SendStatEventWithTags_NullSubs;

	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0xCE02C7);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0xCFDA74);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0xD0818D);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0xD18AA1);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0xDDBC2B);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10CDB75);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0x12743CB);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0x1274408);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0x12E94D7);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0x12E950D);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0x13209AF);
	SendStatEventWithTags_NullSubs.Add(InSDKUtils::GetImageBase() + 0x13209DF);

	for (uintptr_t Addr : SendStatEventWithTags_NullSubs)
		Utils::Rel32(Addr, SendStatEventWithTags);
}