#include "pch.h"
#include "FortniteGame/Public/FortGameModeZone.h"

void FortGameModeZone::CreateAIDirector(AFortGameModeZone* FortGameModeZone)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = 1;

	AActor* FortAIDirector = UWorld::GetWorld()->SpawnActor(AFortAIDirector::StaticClass(), NULL, NULL, &SpawnParams);

	if (FortAIDirector != NULL)
	{
		UGameplayStatics::FinishSpawningActor(FortAIDirector, FTransform());

		FortGameModeZone->AIDirector = Cast<AFortAIDirector>(FortAIDirector);
		FortGameModeZone->AIDirector->Activate();
	}
}

void FortGameModeZone::Setup()
{
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xC98E3B, CreateAIDirector);
}