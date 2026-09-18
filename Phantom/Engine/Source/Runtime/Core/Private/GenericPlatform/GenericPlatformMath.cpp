#include "pch.h"
#include "Core/Public/GenericPlatform/GenericPlatformMath.h"

static int32 GSRandSeed;

void FGenericPlatformMath::SRandInit(int32 Seed)
{
	GSRandSeed = Seed;
}

int32 FGenericPlatformMath::GetRandSeed()
{
	return GSRandSeed;
}

float FGenericPlatformMath::SRand()
{
	GSRandSeed = (GSRandSeed * 196314165) + 907633515;
	union { float f; int32 i; } Result;
	union { float f; int32 i; } Temp;
	const float SRandTemp = 1.0f;
	Temp.f = SRandTemp;
	Result.i = (Temp.i & 0xff800000) | (GSRandSeed & 0x007fffff);
	return FGenericPlatformMath::Fractional(Result.f);
}