#include "pch.h"
#include "Engine/NetDriver.h"

void NetDriver::TickFlush(UNetDriver* NetDriver, float DeltaSeconds)
{
	if (NetDriver->ClientConnections.Num() > 0)
	{
		if (UReplicationDriver* ReplicationDriver = NetDriver->ReplicationDriver)
			ReplicationDriver->ServerReplicateActors();
	}

	if (GetKeyState(VK_F3))
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"startaircraft", NULL);
	}

	if (GetKeyState(VK_F4))
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"demospeed 100", NULL);
	}

	if (GetKeyState(VK_F5))
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"demospeed 1", NULL);
	}

	Originals::TickFlush(NetDriver, DeltaSeconds);
}

void NetDriver::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x27D6330, TickFlush, (void**)&Originals::TickFlush);
}