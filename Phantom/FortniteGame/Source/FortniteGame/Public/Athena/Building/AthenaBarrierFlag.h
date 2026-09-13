#pragma once
#include "pch.h"
#include "Utils.h"

class AthenaBarrierFlag
{
private:
	static AAthenaBarrierObjective* GetObjectiveActor(AAthenaBarrierFlag* Context, FFrame* Stack, AAthenaBarrierObjective** Result);
public:
	static void Setup();
};