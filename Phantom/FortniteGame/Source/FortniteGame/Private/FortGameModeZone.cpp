#include "pch.h"
#include "FortniteGame/Public/FortGameModeZone.h"

void FortGameModeZone::CreateAIDirector(AFortGameModeZone* FortGameModeZone)
{
	UClass* AIDirectorClass = Utils::StaticLoadObject<UClass>(TEXT("/Game/Athena/Deimos/AIDirector/Deimos_AIDirector.Deimos_AIDirector_C"));

	if (AIDirectorClass == NULL)
		AIDirectorClass = AFortAIDirector::StaticClass();

	FortGameModeZone->AIDirector = GWorld->SpawnActor<AFortAIDirector>(FVector(), FRotator(), AIDirectorClass, FortGameModeZone);
	FortGameModeZone->AIDirector->Activate();
}

void FortGameModeZone::FinishWorldInitialization(AFortGameModeZone* FortGameModeZone, AFortWorldManager* WorldManager)
{
	if (WorldManager != NULL)
		WorldManager->FinishWorldInitialization();

	FortGameModeZone->FinishWorldInitialization(WorldManager);

	SetConsoleTitleA("Phantom | Ready");
}

APawn* FortGameModeZone::SpawnDefaultPawnFor_Implementation(AFortGameModeZone* FortGameModeZone, AController* NewPlayer, AActor* StartSpot)
{
	APawn* DefaultPawn = FortGameModeZone->SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	if (AFortPlayerController* FortPlayerController = Cast<AFortPlayerController>(NewPlayer))
	{
		if (FortPlayerController->QuickBars == NULL)
			FortPlayerController->QuickBars = GWorld->SpawnActor<AFortQuickBars>(FVector(), FRotator(), AFortQuickBars::StaticClass(), FortPlayerController);

		if (AFortInventory* WorldInventory = FortPlayerController->GetWorldInventory())
		{
			for (const FItemAndCount& StartingItem : FortGameModeZone->StartingItems)
				WorldInventory->AddItem(StartingItem.Item, StartingItem.Count);

			UAthenaPickaxeItemDefinition* AthenaPickaxeItemDefinition = Utils::StaticFindObject<UAthenaPickaxeItemDefinition>(TEXT("DefaultPickaxe"), ANY_PACKAGE);

			if (AthenaPickaxeItemDefinition != NULL)
				WorldInventory->AddItem(AthenaPickaxeItemDefinition->WeaponDefinition, 1);
		}
	}

	return DefaultPawn;
}

void FortGameModeZone::Setup()
{
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xC98E3B, CreateAIDirector);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0x134F889, FinishWorldInitialization);

	Utils::Virtual(AFortGameModeZone::GetDefaultObj()->VTable, 0xA40 / 8, FinishWorldInitialization);
	Utils::Virtual(AFortGameModeZone::GetDefaultObj()->VTable, 0x610 / 8, SpawnDefaultPawnFor_Implementation);
}