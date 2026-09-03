#include "pch.h"
#include "Utils.h"

AAthenaBigBaseWall* AAthenaBigBaseWall::CreateWall(UWorld* World, TSubclassOf<AAthenaBigBaseWall> BigBaseWallClass, const FVector* MidlineStart, const FVector* MidlineEnd, float ZLevel)
{
	FRotator WallRotation = UKismetMathLibrary::Conv_VectorToRotator(*MidlineEnd - *MidlineStart);
	WallRotation.Pitch = 0.0;
	WallRotation.Roll = 0.0;

	FVector WallLocation = *MidlineStart + (*MidlineEnd - *MidlineStart) * 0.5;
	WallLocation.Z = ZLevel;

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = 1;
	SpawnParams.bDeferConstruction = true;

	AAthenaBigBaseWall* BigBaseWall = Cast<AAthenaBigBaseWall>(World->SpawnActor(BigBaseWallClass.Get(), &WallLocation, &WallRotation, &SpawnParams));

	if (BigBaseWall != NULL)
	{
		UGameplayStatics::FinishSpawningActor(BigBaseWall, UKismetMathLibrary::MakeTransform(WallLocation, WallRotation, FVector(1, 1, 1)));
		return BigBaseWall;
	}

	return NULL;
}