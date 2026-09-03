#pragma once
#include "pch.h"
#include "Utils.h"

class AthenaBarrierFlag
{
private:
	static AAthenaBarrierObjective* GetObjectiveActor(AAthenaBarrierFlag* _this, FFrame& Stack, AAthenaBarrierObjective** Ret);
public:
	static void Setup();
};