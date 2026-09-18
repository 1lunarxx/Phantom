#include "pch.h"
#include "FortniteGame/Public/Missions/FortMission.h"

bool FortMission::InitializeActorsForMissionMatchingItemIdentifyingQuery(AFortMission* FortMission, AFortMission* Mission, const FGameplayTagQuery* TagQuery, TArray<AActor*>* OutActorsInitialized)
{
    printf(__FUNCTION__);

    AFortWorldManager* WorldManager = AFortWorldManager::GetCurrent(Mission);

    if (!WorldManager->MissionManagerRecord.MissionRecords.IsValid())
    {
        FFortMissionRecord MissionRecord = FFortMissionRecord{};

        MissionRecord.MissionGuid = UKismetGuidLibrary::NewGuid();
        MissionRecord.MissionGenerator = FortMission->MissionGenerator;

        MissionRecord.MissionEntry.MissionGenerator = FortMission->MissionGenerator;
        MissionRecord.MissionEntry.MissionInfo = FortMission->MissionGenerator->PrimaryMissionInfo.LoadSynchronous();

        WorldManager->MissionManagerRecord.MissionRecords.Add(MissionRecord);
    }

    return Originals::InitializeActorsForMissionMatchingItemIdentifyingQuery(FortMission, Mission, TagQuery, OutActorsInitialized);
}

void FortMission::Setup()
{
    Utils::Hook(InSDKUtils::GetImageBase() + 0x10C9890, InitializeActorsForMissionMatchingItemIdentifyingQuery, (void**)&Originals::InitializeActorsForMissionMatchingItemIdentifyingQuery);
}