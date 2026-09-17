#include "pch.h"
#include "FortniteGame/Public/STW/FortTaggedActorsManager.h"

UFortTaggedActorsManager* FortTaggedActorsManager::GetCurrent(UWorld* World)
{
	if (AFortGameModeZone* FortGameModeZone = Cast<AFortGameModeZone>(World->AuthorityGameMode))
	{
		return FortGameModeZone->TaggedActorsManager;
	}

	return NULL;
}

// todo: figure out what this does but also later realize its not needed and its for testing or some shit

void FortTaggedActorsManager::FindGameplayTagVolumesForActor(UFortTaggedActorsManager* FortTaggedActorsManager, AActor* TestActor, TArray<IFortGameplayTagVolumeInterface*>* OutVolumes, FGameplayTagContainer* OutTags)
{
}

void FortTaggedActorsManager::Setup()
{
	TArray<uintptr_t> GetCurrent_NullSubs;

	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x12EA243);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xD76A17);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xD7819E);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xDAA900);

	for (uintptr_t GetCurrent_NullSub : GetCurrent_NullSubs)
		Utils::Rel32(GetCurrent_NullSub, GetCurrent);

	TArray<uintptr_t> FindGameplayTagVolumesForActor_NullSubs;

	FindGameplayTagVolumesForActor_NullSubs.Add(InSDKUtils::GetImageBase() + 0x12EA25E);
	FindGameplayTagVolumesForActor_NullSubs.Add(InSDKUtils::GetImageBase() + 0xD76A2E);
	FindGameplayTagVolumesForActor_NullSubs.Add(InSDKUtils::GetImageBase() + 0xD781B5);
	FindGameplayTagVolumesForActor_NullSubs.Add(InSDKUtils::GetImageBase() + 0xDAA91B);

	for (uintptr_t FindGameplayTagVolumesForActor_NullSub : FindGameplayTagVolumesForActor_NullSubs)
		Utils::Rel32(FindGameplayTagVolumesForActor_NullSub, FindGameplayTagVolumesForActor);
}