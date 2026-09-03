#pragma once
#include "pch.h"
#include "Utils.h"

class FortAthenaMutator_Barrier
{
	class Originals
	{
	public:
		static inline void (*BeginPlay)(AFortAthenaMutator_Barrier* FortAthenaMutator_Barrier);
		static inline void (*OnGamePhaseStepChanged)(AFortAthenaMutator_Barrier* FortAthenaMutator_Barrier, EAthenaGamePhaseStep GamePhaseStep);
	};

private:
	static void BeginPlay(AFortAthenaMutator_Barrier* FortAthenaMutator_Barrier);
	static void OnGamePhaseStepChanged(AFortAthenaMutator_Barrier* FortAthenaMutator_Barrier, EAthenaGamePhaseStep GamePhaseStep);
public:
	static void Setup();
};