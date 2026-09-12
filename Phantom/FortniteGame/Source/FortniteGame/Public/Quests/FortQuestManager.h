#pragma once
#include "pch.h"
#include "Utils.h"

struct FQuestFilterValue
{
	uint32 Type;
	uint8 Pad_04[0x4];
	const UEnum* EnumClass;
	bool bResolved;
	uint8 Pad_11[0x7];
	double Number;
	FName Identifier;
	uint8 Pad_24[0x4];
};

struct FQuestFilterExpressionContext
{
	TArray<FQuestFilterValue> CachedValues;
	uint8 Pad_010[0x2A8];
	FGameplayTagContainer CategorizedEntityTags[5];
	bool bLogEvaluationErrors;
	uint8 Pad_359[0x7];
public:
	void Construct(const FGameplayTagContainer* StatEventData, UFortMcpProfileAthena* McpProfileQuest, const AFortPlayerController* PlayerController, bool bInLogEvaluationErrors)
	{
		static void(*Constructor)(FQuestFilterExpressionContext*, const FGameplayTagContainer*, UFortMcpProfileAthena*, const AFortPlayerController*, bool) = decltype(Constructor)(InSDKUtils::GetImageBase() + 0x1333EB0);
		Constructor(this, StatEventData, McpProfileQuest, PlayerController, bInLogEvaluationErrors);
	}
};

struct FFortQuestFilterExpressionEvaluator
{
	bool Evaluate(const void* Code, const FQuestFilterExpressionContext* Context, FText* OutError)
	{
		static bool (*Evaluate)(FFortQuestFilterExpressionEvaluator*, const void*, const FQuestFilterExpressionContext*, FText*) = decltype(Evaluate)(InSDKUtils::GetImageBase() + 0x134BD90);
		return Evaluate((FFortQuestFilterExpressionEvaluator*)(InSDKUtils::GetImageBase() + 0x55B8E40), Code, Context, OutError);
	}
};

class FortQuestManager
{
private:
	static void SendStatEventWithTags(UFortQuestManager* QuestManager, EFortQuestObjectiveStatEvent Type, UObject* TargetObject, FGameplayTagContainer* TargetTags, FGameplayTagContainer* SourceTags, FGameplayTagContainer* ContextTags, int Count);
	static void SendCustomStatEvent(UFortQuestManager* QuestManager, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush);
public:
	static void Setup();
};