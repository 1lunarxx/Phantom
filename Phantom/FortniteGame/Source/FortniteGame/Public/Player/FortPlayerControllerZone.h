#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerControllerZone
{
private:
	static void ServerAcknowledgePossession_Implementation(AFortPlayerControllerZone* FortPlayerControllerZone, APawn* P);
public:
	static void Setup();
};