#include "pch.h"
#include "Engine/World.h"

ENetMode World::InternalGetNetMode(UWorld* World)
{
	return ENetMode::NM_DedicatedServer;
}

bool World::Listen(UWorld* World, FURL& InURL)
{
	if (World->NetDriver)
	{
		return false;
	}

	FName NAME_GameNetDriver = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver"); // temp

	// Create net driver.
	if (GEngine->CreateNamedNetDriver(World, NAME_GameNetDriver, NAME_GameNetDriver))
	{
		World->NetDriver = GEngine->FindNamedNetDriver(World, NAME_GameNetDriver);

		World->NetDriver->World = World;
		World->NetDriver->NetDriverName = NAME_GameNetDriver;

		FLevelCollection* const SourceCollection = World->FindCollectionByType(ELevelCollectionType::DynamicSourceLevels);
		if (SourceCollection)
		{
			SourceCollection->SetNetDriver(World->NetDriver);
		}
		FLevelCollection* const StaticCollection = World->FindCollectionByType(ELevelCollectionType::StaticLevels);
		if (StaticCollection)
		{
			StaticCollection->SetNetDriver(World->NetDriver);
		}
	}

	if (World->NetDriver == NULL)
	{
		return false;
	}

	FString Error;
	if (!World->NetDriver->InitListen(World, &InURL, false, &Error))
	{
		World->NetDriver->SetWorld(NULL);
		World->NetDriver = NULL;
		FLevelCollection* SourceCollection = World->FindCollectionByType(ELevelCollectionType::DynamicSourceLevels);
		if (SourceCollection)
		{
			SourceCollection->SetNetDriver(nullptr);
		}
		FLevelCollection* StaticCollection = World->FindCollectionByType(ELevelCollectionType::StaticLevels);
		if (StaticCollection)
		{
			StaticCollection->SetNetDriver(nullptr);
		}
		return false;
	}

	World->NetDriver->SetWorld(World);

	return true;
}

void World::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x2AB2F80, InternalGetNetMode);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0x2A6324A, Listen);
}