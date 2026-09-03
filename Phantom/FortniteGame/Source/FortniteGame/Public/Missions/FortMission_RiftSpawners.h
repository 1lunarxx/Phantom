#pragma once
#include "pch.h"
#include "Utils.h"

class FortMission_RiftSpawners
{
private:
	static void HandleAISpawned(AFortMission_RiftSpawners* FortMission_RiftSpawners, FFrame& Stack);
public:
	static void Setup();
};