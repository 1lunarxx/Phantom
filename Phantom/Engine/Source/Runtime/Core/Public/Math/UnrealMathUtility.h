#pragma once
#include "Core/Public/GenericPlatform/GenericPlatformMath.h"

struct FMath : public FGenericPlatformMath
{
	// TODO: Add the rest of FMath & FGenericPlatformMath

	static FORCEINLINE float FRandRange(float InMin, float InMax)
	{
		return InMin + (InMax - InMin) * FRand();
	}

	static FORCEINLINE void VRandCone(FVector* Result, FVector* Dir, double ConeHalfAngleRad)
	{
		static void(*VRandCone)(FVector*, FVector*, double) = decltype(VRandCone)(InSDKUtils::GetImageBase() + 0x1797B80);
		VRandCone(Result, Dir, ConeHalfAngleRad);
	}

	static FORCEINLINE FVector VRandCone(FVector Dir, double ConeHalfAngleRad)
	{
		FVector Result;
		VRandCone(&Result, &Dir, ConeHalfAngleRad);

		return Result;
	}
};