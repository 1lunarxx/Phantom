#pragma once
#include "pch.h"
#include "Utils.h"

class FortQuestManager
{
	class Originals
	{
	public:
		static inline void (*SendCustomStatEvent)(UFortQuestManager* FortQuestManager, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush);
	};

private:
	static void SendStatEventWithTags(UFortQuestManager* FortQuestManager, EFortQuestObjectiveStatEvent Type, UObject* TargetObject, FGameplayTagContainer* TargetTags, FGameplayTagContainer* SourceTags, FGameplayTagContainer* ContextTags, int Count);
	static void SendCustomStatEvent(UFortQuestManager* FortQuestManager, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush);
public:
	static void Setup();
};