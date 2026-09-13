#include "pch.h"
#include "FortniteGame/Public/Quests/FortQuestManager.h"

// UNFINISHED

void FortQuestManager::SendStatEventWithTags(UFortQuestManager* QuestManager, EFortQuestObjectiveStatEvent Type, UObject* TargetObject, FGameplayTagContainer* TargetTags, FGameplayTagContainer* SourceTags, FGameplayTagContainer* ContextTags, int Count)
{
	UFortRegisteredPlayerInfo* FortRegisteredPlayerInfo = Cast<UFortRegisteredPlayerInfo>(QuestManager->Outer);

	if (FortRegisteredPlayerInfo == NULL || TargetTags == NULL || FortRegisteredPlayerInfo->GetPlayerController() == NULL)
		return;

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
				FFortQuestObjectiveStatTableRow* QuestObjectiveStatTableRow = Objective.ObjectiveStatHandle.FindRow<FFortQuestObjectiveStatTableRow>(Objective.ObjectiveStatHandle.RowName);
				
				if (QuestObjectiveStatTableRow == NULL || QuestObjectiveStatTableRow->Type != Type)
					continue;

				static void(*watthisdo)(FFortQuestObjectiveStatTableRow*) = decltype(watthisdo)(InSDKUtils::GetImageBase() + 0x13445C0);
				watthisdo(QuestObjectiveStatTableRow);

				if (UFortMcpProfileAthena* BoundProfile = Cast<UFortMcpProfileAthena>(QuestManager->BoundProfile.Get()))
				{
					FQuestFilterExpressionContext QuestFilterExpressionContext;
					QuestFilterExpressionContext.Construct(TargetObject, BoundProfile, FortRegisteredPlayerInfo->GetPlayerController(), NULL, SourceTags, TargetTags, false);

					FText OutError;
					FFortQuestFilterExpressionEvaluator* FortQuestFilterExpressionEvaluator = (FFortQuestFilterExpressionEvaluator*)(InSDKUtils::GetImageBase() + 0x55B8E40);

					bool bEvaluate = FortQuestFilterExpressionEvaluator->Evaluate((void*)((uintptr_t)QuestObjectiveStatTableRow + 0xD8), &QuestFilterExpressionContext, &OutError);

					if (!bEvaluate)
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
				}
			}
		}
	}
}

TMap<UFortQuestManager*, UFortQuestItemDefinition*> ObjectiveOncePerMatchMap;

void FortQuestManager::SendCustomStatEvent(UFortQuestManager* FortQuestManager, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush)
{
	Originals::SendCustomStatEvent(FortQuestManager, ObjectiveStat, Count, bForceFlush);

	UFortRegisteredPlayerInfo* FortRegisteredPlayerInfo = Cast<UFortRegisteredPlayerInfo>(FortQuestManager->Outer);

	if (FortRegisteredPlayerInfo == NULL)
		return;

	AFortPlayerController* FortPlayerController = Cast<AFortPlayerControllerAthena>(FortRegisteredPlayerInfo->GetPlayerController());

	if (FortPlayerController == NULL)
		return;

	for (UFortQuestItem* CurrentQuest : FortQuestManager->CurrentQuests)
	{
		UFortQuestObjectiveInfo* FortQuestObjectiveInfo = CurrentQuest->GetObjectiveInfo(ObjectiveStat);

		if (FortQuestObjectiveInfo == NULL)
			continue;

		UFortQuestItemDefinition* FortQuestItemDefinition = Cast<UFortQuestItemDefinition>(CurrentQuest->ItemDefinition);

		if (FortQuestItemDefinition == NULL)
			continue;

		if (FortPlayerController->ObjectiveOncePerMatch(FortQuestItemDefinition, &ObjectiveOncePerMatchMap))
			return;

		FortQuestObjectiveInfo->AchievedCount += Count;
		FortQuestObjectiveInfo->DisplayDynamicQuestUpdate();

		FFortUpdatedObjectiveStat UpdatedObjectiveStat = FFortUpdatedObjectiveStat{};

		UpdatedObjectiveStat.BackendName = FortQuestObjectiveInfo->BackendName;
		UpdatedObjectiveStat.Quest = FortQuestItemDefinition;
		UpdatedObjectiveStat.StatValue = FortQuestObjectiveInfo->AchievedCount;
		UpdatedObjectiveStat.StatDelta = FortQuestObjectiveInfo->AchievedCount;

		FortPlayerController->UpdateQuest(FortQuestObjectiveInfo->BackendName, FortQuestObjectiveInfo->AchievedCount);

		FortPlayerController->UpdatedObjectiveStats.Add(UpdatedObjectiveStat);
		FortPlayerController->OnRep_UpdatedObjectiveStatsInternal(); // not needed but maybe does something and its a good find.

		FFortQuestObjectiveCompletion FortQuestObjectiveCompletion = FFortQuestObjectiveCompletion{};

		FortQuestObjectiveCompletion.Count = Count;
		FortQuestObjectiveCompletion.StatName = UKismetStringLibrary::Conv_NameToString(FortQuestObjectiveInfo->BackendName);

		FortQuestManager->PendingChanges.Add(FortQuestObjectiveCompletion);

		break;
	}
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
	Utils::Hook(InSDKUtils::GetImageBase() + 0x136D2E0, SendCustomStatEvent, (void**)&Originals::SendCustomStatEvent);
}