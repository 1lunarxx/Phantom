#pragma once
#include "Core/Public/GenericPlatform/GenericPlatformMath.h"

struct FMath : public FGenericPlatformMath
{
	// TODO: Add the rest of FMath & FGenericPlatformMath

	static FORCEINLINE float FRandRange(float InMin, float InMax)
	{
		return InMin + (InMax - InMin) * FRand();
	}
};