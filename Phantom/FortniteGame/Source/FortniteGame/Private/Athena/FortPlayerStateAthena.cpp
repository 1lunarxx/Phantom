#include "pch.h"
#include "Utils.h"

void AFortPlayerStateAthena::InitializeDeathInfo(const FDeathInfo* InDeathInfo)
{
    DeathInfo.FinisherOrDowner = InDeathInfo->FinisherOrDowner;
    DeathInfo.bDBNO = InDeathInfo->bDBNO;
    DeathInfo.DeathCause = InDeathInfo->DeathCause;
    DeathInfo.Distance = InDeathInfo->Distance;
    DeathInfo.bInitialized = InDeathInfo->bInitialized;

    OnRep_DeathInfo();
}

void AFortPlayerStateAthena::IncrementKillStreak()
{
    this->KillScore++;
    this->OnRep_Kills();
}