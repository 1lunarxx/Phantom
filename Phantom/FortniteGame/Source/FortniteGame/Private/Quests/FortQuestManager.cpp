#include "pch.h"
#include "FortniteGame/Public/Quests/FortQuestManager.h"

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

void FortQuestManager::SendStatEventWithTags(UFortQuestManager* QuestManager, EFortQuestObjectiveStatEvent Type, UObject* TargetObject, FGameplayTagContainer* TargetTags, FGameplayTagContainer* SourceTags, FGameplayTagContainer* ContextTags, int Count)
{
/*	UFortRegisteredPlayerInfo* RegisteredPlayerInfo = Cast<UFortRegisteredPlayerInfo>(QuestManager->Outer);

	if (RegisteredPlayerInfo == NULL)
		return;
	
	AFortPlayerController* PlayerController = RegisteredPlayerInfo->GetPlayerController();

	if (PlayerController == NULL)
		return;

	FGameplayTagContainer OutSourceTags;
	FGameplayTagContainer OutContextTags;

	TArray<UFortQuestItem*> OutCurrentQuests;

	QuestManager->GetSourceAndContextTags(&OutSourceTags, &OutContextTags);
	QuestManager->GetCurrentQuests(&OutCurrentQuests);

	ContextTags->AppendTags(&GGameState->CurrentPlaylistInfo.BasePlaylist->GameplayTagContainer);
	SourceTags->AppendTags(&OutSourceTags);

	for (UFortQuestItem* CurrentQuest : QuestManager->CurrentQuests)
	{
		if (CurrentQuest->HasCompletedQuest())
			continue;

		UFortQuestItemDefinition* QuestItemDef = CurrentQuest->GetQuestDefinitionBP();

		if (QuestItemDef == NULL)
			continue;

		for (const FFortMcpQuestObjectiveInfo& Objective : QuestItemDef->Objectives)
		{
			if (CurrentQuest->HasCompletedObjectiveWithName(Objective.BackendName) || QuestManager->HasCompletedObjectiveWithName(QuestItemDef, Objective.BackendName))
				continue;

			FDataTableRowHandle ObjectiveStatHandle = Objective.ObjectiveStatHandle;

			if (ObjectiveStatHandle.DataTable == NULL || !ObjectiveStatHandle.RowName.ComparisonIndex)
				continue;

			if (CurrentQuest->HasCompletedObjective(ObjectiveStatHandle) || QuestManager->HasCompletedObjective(QuestItemDef, ObjectiveStatHandle))
				continue;

			for (const auto& [RowName, RowValue] : ObjectiveStatHandle.DataTable->RowMap)
			{
				if (RowName != ObjectiveStatHandle.RowName)
					continue;

				FFortQuestObjectiveStatTableRow* QuestObjectiveStatTableRow = (FFortQuestObjectiveStatTableRow*)RowValue;

				if (QuestObjectiveStatTableRow == NULL || QuestObjectiveStatTableRow->Type != Type)
					continue;

				if (TargetTags == NULL || !TargetTags->HasAll(QuestObjectiveStatTableRow->TargetTagContainer))
					continue;

				if (SourceTags == NULL || !SourceTags->HasAll(QuestObjectiveStatTableRow->SourceTagContainer))
					continue;

				if (ContextTags == NULL || !ContextTags->HasAll(QuestObjectiveStatTableRow->ContextTagContainer))
					continue;

				UFortMcpProfileAthena* BoundProfile = Cast<UFortMcpProfileAthena>(QuestManager->BoundProfile.Get());

				if (BoundProfile == NULL)
					continue;

				FQuestFilterExpressionContext ConditionContext{};
				ConditionContext.Construct(TargetTags, BoundProfile, PlayerController, false);

				FText OutError;
				FFortQuestFilterExpressionEvaluator* FortQuestFilterExpressionEvaluator = (FFortQuestFilterExpressionEvaluator*)(InSDKUtils::GetImageBase() + 0x55B8E40);

				bool bEvaluateResult = FortQuestFilterExpressionEvaluator->Evaluate((void*)((uintptr_t)QuestObjectiveStatTableRow + 0xD8), &ConditionContext, &OutError);

				if (!bEvaluateResult)
					continue;

				FFortQuestObjectiveCompletion ObjectiveCompletion = FFortQuestObjectiveCompletion{};

				ObjectiveCompletion.StatName = UKismetStringLibrary::Conv_NameToString(Objective.BackendName);
				ObjectiveCompletion.Count = Count;

				QuestManager->PendingChanges.Add(ObjectiveCompletion);
				QuestManager->UpdateStatsForSingleMatchQuests();
				
				break;
			}
		}
	}*/
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

	Utils::Hook(InSDKUtils::GetImageBase() + 0xD81700, SendStatEventWithTags);
}