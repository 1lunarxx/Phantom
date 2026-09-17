#pragma once
#include "pch.h"
#include "Utils.h"

class FortTaggedActorsManager
{
private:
	static UFortTaggedActorsManager* GetCurrent(UWorld* World);
private:
    static void FindGameplayTagVolumesForActor(UFortTaggedActorsManager* FortTaggedActorsManager,  AActor* TestActor, TArray<IFortGameplayTagVolumeInterface*>* OutVolumes, FGameplayTagContainer* OutTags);
public:
	static void Setup();
};