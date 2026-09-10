#include "pch.h"
#include "Engine/NetDriver.h"

void NetDriver::TickFlush(UNetDriver* NetDriver, float DeltaSeconds)
{
	if (UReplicationDriver* ReplicationDriver = NetDriver->ReplicationDriver)
	{
		ReplicationDriver->ServerReplicateActors();
	}

	Originals::TickFlush(NetDriver, DeltaSeconds);
}

UReplicationDriver* NetDriver::ConditionalCreateReplicationDriver(UNetDriver* ForNetDriver, UWorld* World)
{
	if (AFortGameMode* FortGameMode = Cast<AFortGameMode>(World->AuthorityGameMode))
		FortGameMode->bEnableReplicationGraph = true;

	return Originals::ConditionalCreateReplicationDriver(ForNetDriver, World);
}

void NetDriver::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x27D6330, TickFlush, (void**)&Originals::TickFlush);
	Utils::Hook(InSDKUtils::GetImageBase() + 0x11ED080, ConditionalCreateReplicationDriver, (void**)&Originals::ConditionalCreateReplicationDriver);
}