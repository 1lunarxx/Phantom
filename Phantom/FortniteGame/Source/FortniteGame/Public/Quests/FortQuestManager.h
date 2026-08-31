#pragma once
#include "pch.h"
#include "Utils.h"

class FortQuestManager
{
private:
	static void SendStatEventWithTags(UFortQuestManager* QuestManager, EFortQuestObjectiveStatEvent Type, UObject* TargetObject, FGameplayTagContainer* TargetTags, FGameplayTagContainer* SourceTags, FGameplayTagContainer* ContextTags, int Count);
public:
	static void Setup();
};