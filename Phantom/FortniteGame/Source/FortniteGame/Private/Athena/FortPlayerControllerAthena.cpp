#include "pch.h"
#include "FortniteGame/Public/Athena/FortPlayerControllerAthena.h"

void FortPlayerControllerAthena::ServerRestartPlayer_Implementation(AFortPlayerControllerAthena* FortPlayerControllerAthena)
{
	static void(*ServerRestartPlayer)(AFortPlayerControllerZone*) = decltype(ServerRestartPlayer)(AFortPlayerControllerZone::GetDefaultObj()->VTable[0x7E8 / 8]);
	ServerRestartPlayer(FortPlayerControllerAthena);
}

void FortPlayerControllerAthena::Setup()
{
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x7E8 / 8, ServerRestartPlayer_Implementation);
}