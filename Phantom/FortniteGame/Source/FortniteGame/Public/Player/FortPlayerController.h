#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerController
{
private:
	static void ServerExecuteInventoryItem_Implementation(AFortPlayerController* FortPlayerController, FGuid* ItemGuid);
public:
	static void Setup();
};