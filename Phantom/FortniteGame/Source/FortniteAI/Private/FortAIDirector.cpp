#include "pch.h"
#include "FortniteAI/Public/FortAIDirector.h"

AFortAIDirector* FortAIDirector::GetCurrent(UWorld* World)
{
	if (GGameMode->AIDirector == NULL)
		printf("null sutff\n");

	return GGameMode->AIDirector;
}

void FortAIDirector::Setup()
{
	TArray<uintptr_t> GetCurrent_NullSubs;

	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xD7627D); // HandleDamaged
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xDC95EB); // FinishTrigger
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xF1ADE2); // GetFriendlyActors

	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10B9022); // CreateEncounterSequence
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10B9141); // CreateEncounterSequenceByIndex
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D3CB1); // SomeThingWithRift

	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D457E); // StartMissionAIEncounter
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D4912); // Star`tMissionAIEncounterFromGeneratedProfile
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D5635); // StopEncounterSequence
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D5727); // StopMissionAIEncounter
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10B80A0); // CheckSpawnerShutdown
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10B9A9F); // SetCalendarAllowsActive
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10BA186); // RedirectForAlotOfSubs
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x1241F3C); // HandleDeathNotifications
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xDC95EB); // MakeBotNoiseEventAlongSegment
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xBA1FF7); // UAthenaAISense_Hearing::Update
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x12F3A0A); // UpdateNearbyEncounters
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xB22192); // UFortBTService_UpdateBotMissionGoal::TickNode

	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xAE6171);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xB3487A); // FUndermineHelpers::AreBuildingsInRange
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xB49AEE); // UFortQueryTest_HasNearbyEncounterGoals::RunTest

	for (uintptr_t GetCurrent_NullSub : GetCurrent_NullSubs)
		Utils::Rel32(GetCurrent_NullSub, GetCurrent);
}