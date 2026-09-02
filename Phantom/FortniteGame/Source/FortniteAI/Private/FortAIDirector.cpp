#include "pch.h"
#include "FortniteAI/Public/FortAIDirector.h"

AFortAIDirector* FortAIDirector::GetCurrent(UWorld* World)
{
	return GGameMode->AIDirector;
}

void FortAIDirector::Setup()
{
	TArray<uintptr_t> GetCurrent_NullSubs;

	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10B9022);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10B9141);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D3CB1);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D4912);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D5635);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D5727);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10BA186);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x1241F3C);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10B9A9F);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x12F3A0A);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0x10D457E);

	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xBA1FF7);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xAE6171);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xD7627D);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xDC95EB);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xDC95EB);
	GetCurrent_NullSubs.Add(InSDKUtils::GetImageBase() + 0xB3487A);

	for (uintptr_t GetCurrent_NullSub : GetCurrent_NullSubs)
		Utils::Rel32(GetCurrent_NullSub, GetCurrent);
}