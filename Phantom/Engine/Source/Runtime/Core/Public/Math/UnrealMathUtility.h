#pragma once
#include "Core/Public/GenericPlatform/GenericPlatformMath.h"

struct FMath : public FGenericPlatformMath
{
	// TODO: Add the rest of FMath & FGenericPlatformMath

	static FORCEINLINE float FRandRange(float InMin, float InMax)
	{
		return InMin + (InMax - InMin) * FRand();
	}

	static FORCEINLINE int32 RandRange(int32 InMin, int32 InMax)
	{
		return InMin + Rand() % (InMax - InMin + 1);
	}

	/** Clamps X to be between Min and Max, inclusive */
	template< class T >
	static FORCEINLINE T Clamp(const T X, const T Min, const T Max)
	{
		return X < Min ? Min : X < Max ? X : Max;
	}

	/** Performs a linear interpolation between two values, Alpha ranges from 0-1 */
	template< class T, class U >
	static FORCEINLINE T Lerp(const T& A, const T& B, const U& Alpha)
	{
		return (T)(A + Alpha * (B - A));
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