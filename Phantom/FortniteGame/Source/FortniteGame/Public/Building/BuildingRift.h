#pragma once
#include "pch.h"
#include "Utils.h"

class BuildingRift
{
	class Originals
	{
	public:
		static inline void (*BeginPlay)(ABuildingRift* BuildingRift);
	};

private:
	static void BeginPlay(ABuildingRift* BuildingRift);
public:
	static void Setup();
};