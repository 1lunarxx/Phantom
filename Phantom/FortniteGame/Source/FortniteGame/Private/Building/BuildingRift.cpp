#include "pch.h"
#include "FortniteGame/Public/Building/BuildingRift.h"
#include "FortniteGame/Public/Items/FortLootPackage.h"

void BuildingRift::BeginPlay(ABuildingRift* BuildingRift)
{
	printf(__FUNCTION__);

	Originals::BeginPlay(BuildingRift);
}

void BuildingRift::Setup()
{
	Utils::Virtual(ABuildingRift::GetDefaultObj()->VTable, 0x2F8 / 8, BeginPlay, (void**)&Originals::BeginPlay);
}