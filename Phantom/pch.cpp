// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

FName::FName(FString String)
{
	*this = UKismetStringLibrary::Conv_StringToName(String);
}

FRotator FQuat::Rotator()
{
	static FRotator(*Rotator)(FQuat*) = decltype(Rotator)(InSDKUtils::GetImageBase() + 0x1793430);
	return Rotator(this);
}

FVector FAircraftFlightInfo::GetFlightEnd()
{
	FVector Direction = UKismetMathLibrary::Conv_RotatorToVector(FlightStartRotation);

	const double SizeSquared = Direction.X * Direction.X + Direction.Y * Direction.Y + Direction.Z * Direction.Z;
	if (SizeSquared > 1e-8)
	{
		Direction /= std::sqrt(SizeSquared);
	}
	else
	{
		Direction = FVector();
	}

	return FlightStartLocation + Direction * (FlightSpeed * TimeTillFlightEnd);
}

void FFortItemEntry::SetLoadedAmmo(int InCount)
{
	if (InCount != LoadedAmmo)
	{
		LoadedAmmo = InCount;

		if (AFortInventory* FortInventory = ParentInventory.Get())
			FortInventory->SetItemRequiresUpdate(this);

		bIsDirty = 1;
	}
}

void FGameplayMutatorObjectData::PostReplicatedAdd(struct FGameplayMutatorObjectDataArray* InArraySerializer)
{
	InArraySerializer->MarkItemDirty(*this);
	InArraySerializer->ObjectDataList.Add(*this);
}