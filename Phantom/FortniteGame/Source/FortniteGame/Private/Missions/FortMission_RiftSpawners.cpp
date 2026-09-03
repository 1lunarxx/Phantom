#include "pch.h"
#include "FortniteGame/Public/Missions/FortMission_RiftSpawners.h"

void FortMission_RiftSpawners::HandleAISpawned(AFortMission_RiftSpawners* FortMission_RiftSpawners, FFrame& Stack)
{
	Stack.IncrementCode();
/*	UFortAIEncounterInfo* Encounter, AFortAIPawn* SpawnedEnemy*/
	printf(__FUNCTION__);
}

void FortMission_RiftSpawners::Setup()
{
	Utils::Exec(TEXT("/Script/FortniteGame.FortMission_RiftSpawners.HandleAISpawned"), HandleAISpawned);
}