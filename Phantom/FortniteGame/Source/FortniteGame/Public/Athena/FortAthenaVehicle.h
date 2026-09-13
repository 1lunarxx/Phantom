#pragma once
#include "pch.h"
#include "Utils.h"

class FortAthenaVehicle
{
private:
	static void ServerUpdatePhysicsParams(AFortAthenaVehicle* FortAthenaVehicle, FReplicatedAthenaVehiclePhysicsState& InState);
public:
	static void Setup();
};