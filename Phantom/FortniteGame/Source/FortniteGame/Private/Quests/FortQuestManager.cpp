#include "pch.h"
#include "FortniteGame/Public/Quests/FortQuestManager.h"

void FortQuestManager::SendStatEventWithTags(UFortQuestManager* QuestManager, EFortQuestObjectiveStatEvent Type, UObject* TargetObject, FGameplayTagContainer* TargetTags, FGameplayTagContainer* SourceTags, FGameplayTagContainer* ContextTags, int Count)
{
	UFortRegisteredPlayerInfo* FortRegisteredPlayerInfo = Cast<UFortRegisteredPlayerInfo>(QuestManager->Outer);

	if (FortRegisteredPlayerInfo == NULL || TargetTags == NULL)
		return;

	printf(__FUNCTION__);

	for (UFortQuestItem* CurrentQuest : QuestManager->CurrentQuests)
	{
		if (CurrentQuest->HasCompletedQuest())
			continue;

		UFortQuestItemDefinition* QuestItemDefinition = Cast<UFortQuestItemDefinition>(CurrentQuest->ItemDefinition);

		if (QuestItemDefinition == NULL)
			continue;

		for (FFortMcpQuestObjectiveInfo& Objective : QuestItemDefinition->Objectives)
		{
			if (Objective.Stage == -1 || Objective.Stage == CurrentQuest->CurrentStage)
			{	
				FFortQuestObjectiveStatTableRow* QuestObjectiveStatTableRow = Objective.ObjectiveStatHandle.FindRow<FFortQuestObjectiveStatTableRow>(QuestItemDefinition->Name);
				
				if (QuestObjectiveStatTableRow == NULL)
					continue;

				static void(*watthisdo)(FFortQuestObjectiveStatTableRow*) = decltype(watthisdo)(InSDKUtils::GetImageBase() + 0x13445C0);
				watthisdo(QuestObjectiveStatTableRow);

				if (UFortMcpProfileAthena* BoundProfile = Cast<UFortMcpProfileAthena>(QuestManager->BoundProfile.Get()))
				{
					FQuestFilterExpressionContext QuestFilterExpressionContext;
					QuestFilterExpressionContext.Construct(TargetTags, BoundProfile, FortRegisteredPlayerInfo->GetPlayerController(), false);

					FText OutError;
					FFortQuestFilterExpressionEvaluator* FortQuestFilterExpressionEvaluator = (FFortQuestFilterExpressionEvaluator*)(InSDKUtils::GetImageBase() + 0x55B8E40);

					if (!FortQuestFilterExpressionEvaluator->Evaluate((void*)((uintptr_t)QuestObjectiveStatTableRow + 0xD8), &QuestFilterExpressionContext, &OutError))
						continue;

					FString BackendName = UKismetStringLibrary::Conv_NameToString(Objective.BackendName);
					FFortQuestObjectiveCompletion* PendingChange = QuestManager->GetPendingChange(&BackendName);

					if (PendingChange == NULL)
					{
						FFortQuestObjectiveCompletion FortQuestObjectiveCompletion = FFortQuestObjectiveCompletion{};

						FortQuestObjectiveCompletion.Count = Count;
						FortQuestObjectiveCompletion.StatName = BackendName;

						QuestManager->PendingChanges.Add(FortQuestObjectiveCompletion);
					}

					printf("gay\n");

					std::cout << "uh: " << QuestItemDefinition->GetFullName() << std::endl;

/*					if (QuestManager->GetChangesInPendingSaveRequests(NULL)) // this should be a if statement where it checks if this is greater then Objective->Count but uh lazy
					{
						int32 InCount = 0;

						int* CompletionCount = CurrentQuest->CompletionCounts.Find(Objective.BackendName);

						if (CompletionCount)
							InCount = *CompletionCount;

						if (InCount < Objective.Count)
						{
							FString BackendName = UKismetStringLibrary::Conv_NameToString(Objective.BackendName);
							FFortQuestObjectiveCompletion* PendingChange = QuestManager->GetPendingChange(&BackendName);

							if (PendingChange == NULL)
							{
								FFortQuestObjectiveCompletion FortQuestObjectiveCompletion = FFortQuestObjectiveCompletion{};

								QuestManager->PendingChanges.Add(FortQuestObjectiveCompletion);
							}
						}
					}*/
				}
			}
		}
	}
}

void FortQuestManager::SendCustomStatEvent(UFortQuestManager* QuestManager, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush)
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