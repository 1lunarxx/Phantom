#include "pch.h"
#include "Engine/NetDriver.h"

#include "Core/Public/Math/UnrealMathUtility.h"
#include "Core/Public/Templates/Sorting.h"

int32 UNetDriver::ServerReplicateActors_PrepConnections(const float DeltaSeconds)
{
	int32 NumClientsToTick = ClientConnections.Num();

	bool bFoundReadyConnection = false;

	for (int32 ConnIdx = 0; ConnIdx < ClientConnections.Num(); ConnIdx++)
	{
		UNetConnection* Connection = ClientConnections[ConnIdx];

		if (Connection == NULL)
		{
			continue;
		}

		if (Connection->State != USOCK_Pending && Connection->State != USOCK_Open && Connection->State == USOCK_Closed)
		{
			continue;
		}

		AActor* OwningActor = Connection->OwningActor;
		if (OwningActor != NULL && Connection->State == USOCK_Open && (Connection->Driver->Time - Connection->LastReceiveTime < 1.5f))
		{
			if (World != OwningActor->GetWorld())
			{
				continue;
			}

			bFoundReadyConnection = true;

			AActor* DesiredViewTarget = OwningActor;
			if (Connection->PlayerController)
			{
				if (AActor* ViewTarget = Connection->PlayerController->GetViewTarget())
				{
					if (ViewTarget->GetWorld())
					{
						DesiredViewTarget = ViewTarget;
					}
				}
			}
			Connection->ViewTarget = DesiredViewTarget;

			for (int32 ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
			{
				UNetConnection* Child = Connection->Children[ChildIdx];
				APlayerController* ChildPlayerController = Child->PlayerController;
				if (ChildPlayerController != NULL)
				{
					Child->ViewTarget = ChildPlayerController->GetViewTarget();
				}
				else
				{
					Child->ViewTarget = NULL;
				}
			}
		}
		else
		{
			Connection->ViewTarget = NULL;
			for (int32 ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
			{
				Connection->Children[ChildIdx]->ViewTarget = NULL;
			}
		}
	}

	return bFoundReadyConnection ? NumClientsToTick : 0;
}

void UNetDriver::ServerReplicateActors_BuildConsiderList(TArray<FNetworkObjectInfo*>& OutConsiderList, const float ServerTickTime)
{
	int32 NumInitiallyDormant = 0;

	const bool bUseAdapativeNetFrequency = IsAdaptiveNetUpdateFrequencyEnabled();

	TArray<AActor*> ActorsToRemove;

	float TimeSeconds = UGameplayStatics::GetTimeSeconds(World);

	for (const TSharedPtr<FNetworkObjectInfo>& ObjectInfo : GetNetworkObjectList()->GetActiveObjects())
	{
		FNetworkObjectInfo* ActorInfo = ObjectInfo.Get();

		if (!ActorInfo->bPendingNetUpdate && TimeSeconds <= ActorInfo->NextUpdateTime)
		{
			continue;
		}

		AActor* Actor = ActorInfo->Actor;

		if (Actor == NULL)
		{
			continue;
		}

/*		if (Actor->IsPendingKillPending())
		{
			ActorsToRemove.Add(Actor);
			continue;
		}*/

		if (Actor->GetRemoteRole() == ENetRole::ROLE_None)
		{
			ActorsToRemove.Add(Actor);
			continue;
		}

		if (Actor->NetDriverName != NetDriverName)
		{
			continue;
		}

		if (!Actor->IsActorInitialized())
		{
			continue;
		}

		ULevel* Level = Actor->GetLevel();
		if (Level->HasVisibilityChangeRequestPending() || Level->bIsAssociatingLevel)
		{
			continue;
		}

		if (Actor->NetDormancy == ENetDormancy::DORM_Initial && Actor->IsNetStartupActor())
		{
			NumInitiallyDormant++;
			ActorsToRemove.Add(Actor);
			continue;
		}

		if (!Actor->NeedsLoadForClient())
			continue; 

		if (World != Actor->GetWorld())
			continue;

		if (ActorInfo->LastNetReplicateTime == 0)
		{
			ActorInfo->LastNetReplicateTime = TimeSeconds;
			ActorInfo->OptimalNetUpdateDelta = 1.0f / Actor->NetUpdateFrequency;
		}

		const float ScaleDownStartTime = 2.0f;
		const float ScaleDownTimeRange = 5.0f;

		const float LastReplicateDelta = TimeSeconds - ActorInfo->LastNetReplicateTime;

		if (LastReplicateDelta > ScaleDownStartTime)
		{
			if (Actor->MinNetUpdateFrequency == 0.0f)
			{
				Actor->MinNetUpdateFrequency = 2.0f;
			}

			const float MinOptimalDelta = 1.0f / Actor->NetUpdateFrequency;
			const float MaxOptimalDelta = FMath::Max(1.0f / Actor->MinNetUpdateFrequency, MinOptimalDelta);

			const float Alpha = FMath::Clamp((LastReplicateDelta - ScaleDownStartTime) / ScaleDownTimeRange, 0.0f, 1.0f);
			ActorInfo->OptimalNetUpdateDelta = FMath::Lerp(MinOptimalDelta, MaxOptimalDelta, Alpha);
		}

		if (!ActorInfo->bPendingNetUpdate)
		{
			const float NextUpdateDelta = bUseAdapativeNetFrequency ? ActorInfo->OptimalNetUpdateDelta : 1.0f / Actor->NetUpdateFrequency;

			ActorInfo->NextUpdateTime = TimeSeconds + FMath::SRand() * ServerTickTime + NextUpdateDelta;
			ActorInfo->LastNetUpdateTime = Time;
		}

		ActorInfo->bPendingNetUpdate = false;

		if (OutConsiderList.Num() > OutConsiderList.Max())
			continue;

		OutConsiderList.Add(ActorInfo);

		Actor->CallPreReplication(this);
	}

	for (AActor* Actor : ActorsToRemove)
	{
		RemoveNetworkActor(Actor);
	}
}

static FORCEINLINE bool IsActorRelevantToConnection(AActor* Actor, const TArray<FNetViewer>& ConnectionViewers)
{
	for (int32 viewerIdx = 0; viewerIdx < ConnectionViewers.Num(); viewerIdx++)
	{
		if (Actor->IsNetRelevantFor(ConnectionViewers[viewerIdx].InViewer, ConnectionViewers[viewerIdx].ViewTarget, ConnectionViewers[viewerIdx].ViewLocation))
		{
			return true;
		}
	}

	return false;
}

static FORCEINLINE UNetConnection* IsActorOwnedByAndRelevantToConnection(AActor* Actor, const TArray<FNetViewer>& ConnectionViewers, bool& bOutHasNullViewTarget)
{
	AActor* ActorOwner = Actor->GetNetOwner();

	bOutHasNullViewTarget = false;

	for (int i = 0; i < ConnectionViewers.Num(); i++)
	{
		UNetConnection* ViewerConnection = ConnectionViewers[i].Connection;

		if (ViewerConnection->ViewTarget == nullptr)
		{
			bOutHasNullViewTarget = true;
		}

		if (ActorOwner == ViewerConnection->PlayerController ||
			(ViewerConnection->PlayerController && ActorOwner == ViewerConnection->PlayerController->GetPawn()) ||
			(ViewerConnection->ViewTarget && ViewerConnection->ViewTarget->IsRelevancyOwnerFor(Actor, ActorOwner, ViewerConnection->OwningActor)))
		{
			return ViewerConnection;
		}
	}

	return nullptr;
}

static FORCEINLINE bool IsActorDormant(FNetworkObjectInfo* ActorInfo, const TWeakObjectPtr<UNetConnection>& Connection)
{
	for (TWeakObjectPtr<UNetConnection> DormantConnection : ActorInfo->DormantConnections)
	{
		if (DormantConnection == Connection)
			return true;
	}

	return false;
}

static FORCEINLINE bool ShouldActorGoDormant(AActor* Actor, const TArray<FNetViewer>& ConnectionViewers, UActorChannel* Channel, const float Time, const bool bLowNetBandwidth)
{
	if (Actor->NetDormancy <= ENetDormancy::DORM_Awake || !Channel || Channel->IsPendingDormancy() || Channel->Dormant)
	{
		return false;
	}

	if (Actor->NetDormancy == ENetDormancy::DORM_DormantPartial)
	{
		for (int32 viewerIdx = 0; viewerIdx < ConnectionViewers.Num(); viewerIdx++)
		{
			if (!Actor->GetNetDormancy(ConnectionViewers[viewerIdx].ViewLocation, ConnectionViewers[viewerIdx].ViewDir, ConnectionViewers[viewerIdx].InViewer, ConnectionViewers[viewerIdx].ViewTarget, Channel, Time, bLowNetBandwidth))
			{
				return false;
			}
		}
	}

	return true;
}

int32 UNetDriver::ServerReplicateActors_PrioritizeActors(UNetConnection* Connection, const TArray<FNetViewer>& ConnectionViewers, const TArray<FNetworkObjectInfo*> ConsiderList, const bool bCPUSaturated, FActorPriority*& OutPriorityList, FActorPriority**& OutPriorityActors)
{
	NetTag++;
	Connection->TickCount++;

	for (int32 j = 0; j < Connection->SentTemporaries.Num(); j++)
	{
		Connection->SentTemporaries[j]->NetTag = NetTag;
	}

	if (World != Connection->OwningActor->GetWorld())
	{
		return 0;
	}

	int32 FinalSortedCount = 0;
	int32 DeletedCount = 0;

	TWeakObjectPtr<UNetConnection> WeakConnection(Connection);

	const int32 MaxSortedActors = ConsiderList.Num() + GetDestroyedStartupOrDormantActors().Num();
	if (MaxSortedActors > 0)
	{
/*		OutPriorityList = new (FMemStack::Get(), MaxSortedActors) FActorPriority;
		OutPriorityActors = new (FMemStack::Get(), MaxSortedActors) FActorPriority*;*/

		OutPriorityList = new FActorPriority[MaxSortedActors];
		OutPriorityActors = new FActorPriority * [MaxSortedActors];

		if (World != Connection->ViewTarget->GetWorld())
		{
			return 0;
		}

		AGameNetworkManager* const NetworkManager = World->NetworkManager;
		const bool bLowNetBandwidth = NetworkManager ? NetworkManager->IsInLowBandwidthMode() : false;

		for (FNetworkObjectInfo* ActorInfo : ConsiderList)
		{
			AActor* Actor = ActorInfo->Actor;

			UActorChannel* Channel = Connection->FindActorChannelRef(ActorInfo->WeakActor);

			if (!Channel)
			{
				if (!IsLevelInitializedForActor(Actor, Connection))
				{
					continue;
				}

				if (!IsActorRelevantToConnection(Actor, ConnectionViewers))
				{
					continue;
				}
			}

			UNetConnection* PriorityConnection = Connection;

			if (Actor->bOnlyRelevantToOwner)
			{
				bool bHasNullViewTarget = false;

				PriorityConnection = IsActorOwnedByAndRelevantToConnection(Actor, ConnectionViewers, bHasNullViewTarget);

				if (PriorityConnection == nullptr)
				{
					if (!bHasNullViewTarget && Channel != NULL && Time - Channel->RelevantTime >= RelevantTimeout)
					{
						Channel->Close();
					}

					continue;
				}
			}
			else if (GSetNetDormancyEnabled != 0)
			{
/*				if (IsActorDormant(ActorInfo, WeakConnection))
				{
					continue;
				}

				if (ShouldActorGoDormant(Actor, ConnectionViewers, Channel, Time, bLowNetBandwidth))
				{
					Channel->StartBecomingDormant();
				}*/
			}

			if (Actor->NetTag != NetTag)
			{
				Actor->NetTag = NetTag;

				OutPriorityList[FinalSortedCount] = FActorPriority(PriorityConnection, Channel, ActorInfo, ConnectionViewers, bLowNetBandwidth);
				OutPriorityActors[FinalSortedCount] = OutPriorityList + FinalSortedCount;

				FinalSortedCount++;
			}
		}

		for (auto It = Connection->GetDestroyedStartupOrDormantActorGUIDs().CreateConstIterator(); It; ++It)
		{
			TUniquePtr<FActorDestructionInfo>* DInfo = GetDestroyedStartupOrDormantActors().Find(*It);

			if (DInfo == NULL)
				continue;

			OutPriorityList[FinalSortedCount] = FActorPriority(Connection, DInfo->Ptr, ConnectionViewers);
			OutPriorityActors[FinalSortedCount] = OutPriorityList + FinalSortedCount;
			FinalSortedCount++;
			DeletedCount++;
		}

		Sort(OutPriorityActors, FinalSortedCount, FCompareFActorPriority());
	}

	return FinalSortedCount;
}

int32 UNetDriver::ServerReplicateActors_ProcessPrioritizedActors(UNetConnection* Connection, const TArray<FNetViewer>& ConnectionViewers, FActorPriority** PriorityActors, const int32 FinalSortedCount, int32& OutUpdated)
{
	int32 ActorUpdatesThisConnection = 0;
	int32 ActorUpdatesThisConnectionSent = 0;
	int32 FinalRelevantCount = 0;

	float TimeSeconds = UGameplayStatics::GetTimeSeconds(World);

	if (!Connection->IsNetReady(0))
	{
		return 0;
	}

	TSet<FName> ClientVisibleLevelNames = *(TSet<FName>*)(__int64(Connection) + 0x1768);

	for (int32 j = 0; j < FinalSortedCount; j++)
	{
		FNetworkObjectInfo* ActorInfo = PriorityActors[j]->ActorInfo;

		if (ActorInfo == NULL && PriorityActors[j]->DestructionInfo)
		{
			if (PriorityActors[j]->DestructionInfo->StreamingLevelName != NAME_None && !ClientVisibleLevelNames.Contains(PriorityActors[j]->DestructionInfo->StreamingLevelName))
			{
				continue;
			}

			UActorChannel* Channel = (UActorChannel*)Connection->CreateChannel(NAME_Actor, CHTYPE_Actor, -1);
			if (Channel)
			{
				FinalRelevantCount++;

				Channel->SetChannelActorForDestroy(PriorityActors[j]->DestructionInfo);
				Connection->RemoveDestructionInfo(PriorityActors[j]->DestructionInfo);
			}
			continue;
		}

		UActorChannel* Channel = PriorityActors[j]->Channel;

		if (!Channel || Channel->Actor) 
		{
			AActor* Actor = ActorInfo->Actor;
			bool bIsRelevant = false;

			const bool bLevelInitializedForActor = IsLevelInitializedForActor(Actor, Connection);

			if (bLevelInitializedForActor)
			{
				if (!Actor->GetTearOff() && (!Channel || Time - Channel->RelevantTime > 1.f))
				{
					if (IsActorRelevantToConnection(Actor, ConnectionViewers))
					{
						bIsRelevant = true;
					}
				}
			}

			const bool bIsRecentlyRelevant = bIsRelevant || (Channel && Time - Channel->RelevantTime < RelevantTimeout) || ActorInfo->bForceRelevantNextUpdate;

			ActorInfo->bForceRelevantNextUpdate = false;

			if (bIsRecentlyRelevant)
			{
				FinalRelevantCount++;

				if (Channel == NULL && GetGuidCache()->SupportsObject(Actor->Class))
				{
					if (bLevelInitializedForActor)
					{
						Channel = (UActorChannel*)Connection->CreateChannel(NAME_Actor, CHTYPE_Actor);
						if (Channel)
						{
							Channel->SetChannelActor(Actor);
						}
					}
					else if (Actor->NetUpdateFrequency < 1.0f)
					{
						ActorInfo->NextUpdateTime = TimeSeconds + 0.2f * FMath::FRand();
					}
				}

				if (Channel)
				{					
					if (bIsRelevant)
					{
						Channel->RelevantTime = Time + 0.5f * FMath::SRand();
					}
					if (Channel->IsNetReady(0))
					{
						double ChannelLastNetUpdateTime = Channel->LastUpdateTime;

						if (Channel->ReplicateActor())
						{
							const float MinOptimalDelta = 1.0f / Actor->NetUpdateFrequency;
							const float MaxOptimalDelta = FMath::Max(1.0f / Actor->MinNetUpdateFrequency, MinOptimalDelta);
							const float DeltaBetweenReplications = (TimeSeconds - ActorInfo->LastNetReplicateTime);

							ActorInfo->OptimalNetUpdateDelta = FMath::Clamp(DeltaBetweenReplications * 0.7f, MinOptimalDelta, MaxOptimalDelta);
							ActorInfo->LastNetReplicateTime = TimeSeconds;
						}

						ActorUpdatesThisConnection++;
						OutUpdated++;
					}

					if (!Connection->IsNetReady(0))
					{
						return j;
					}
				}
			}

			if ((!bIsRecentlyRelevant || Actor->GetTearOff()) && Channel != NULL)
			{
				if (!bLevelInitializedForActor || !Actor->IsNetStartupActor())
				{
					Channel->Close();
				}
			}
		}
	}

	return FinalSortedCount;
}

int32 UNetDriver::ServerReplicateActors(float DeltaSeconds)
{
	if (ClientConnections.Num() == 0)
	{
		return 0;
	}

	if (World == NULL)
	{
		return 0;
	}

	ReplicationFrame++;

	int32 Updated = 0;

	const int32 NumClientsToTick = ServerReplicateActors_PrepConnections(DeltaSeconds);

	if (NumClientsToTick == 0)
	{
		return 0;
	}

	AWorldSettings* WorldSettings = World->GetWorldSettings();

	bool bCPUSaturated = false;
	float ServerTickTime = 120.f;
	if (ServerTickTime == 0.f)
	{
		ServerTickTime = DeltaSeconds;
	}
	else
	{
		ServerTickTime = 1.f / ServerTickTime;
		bCPUSaturated = DeltaSeconds > 1.2f * ServerTickTime;
	}

	TArray<FNetworkObjectInfo*> ConsiderList;
	ConsiderList.Reserve(GetNetworkObjectList()->GetActiveObjects().Num());

	ServerReplicateActors_BuildConsiderList(ConsiderList, ServerTickTime);

	for (int32 i = 0; i < ClientConnections.Num(); i++)
	{
		UNetConnection* Connection = ClientConnections[i];

		if (Connection == NULL)
		{
			continue;
		}

		if (i >= NumClientsToTick)
		{
			for (int32 ConsiderIdx = 0; ConsiderIdx < ConsiderList.Num(); ConsiderIdx++)
			{
				AActor* Actor = ConsiderList[ConsiderIdx]->Actor;
				if (Actor != NULL && !ConsiderList[ConsiderIdx]->bPendingNetUpdate)
				{
					UActorChannel* Channel = Connection->FindActorChannelRef(ConsiderList[ConsiderIdx]->WeakActor);
					if (Channel != NULL && Channel->LastUpdateTime < ConsiderList[ConsiderIdx]->LastNetUpdateTime)
					{
						ConsiderList[ConsiderIdx]->bPendingNetUpdate = true;
					}
				}
			}
			*(bool*)(__int64(Connection) + 0x239) = false;
		}
		else if (Connection->ViewTarget)
		{
			TArray<FNetViewer>& ConnectionViewers = WorldSettings->ReplicationViewers;

			ConnectionViewers.Reset();
			ConnectionViewers.Add(FNetViewer(Connection, DeltaSeconds));

			for (int32 ViewerIndex = 0; ViewerIndex < Connection->Children.Num(); ViewerIndex++)
			{
				if (Connection->Children[ViewerIndex]->ViewTarget != NULL)
				{
					ConnectionViewers.Add(FNetViewer(Connection->Children[ViewerIndex], DeltaSeconds));
				}
			}

			if (Connection->PlayerController)
			{
				Connection->PlayerController->SendClientAdjustment();
			}

			for (int32 ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
			{
				if (Connection->Children[ChildIdx]->PlayerController != NULL)
				{
					Connection->Children[ChildIdx]->PlayerController->SendClientAdjustment();
				}
			}

			FActorPriority* PriorityList = NULL;
			FActorPriority** PriorityActors = NULL;

			const int32 FinalSortedCount = ServerReplicateActors_PrioritizeActors(Connection, ConnectionViewers, ConsiderList, bCPUSaturated, PriorityList, PriorityActors);

			const int32 LastProcessedActor = ServerReplicateActors_ProcessPrioritizedActors(Connection, ConnectionViewers, PriorityActors, FinalSortedCount, Updated);

			for (int32 k = LastProcessedActor; k < FinalSortedCount; k++)
			{
				if (!PriorityActors[k]->ActorInfo)
				{
					continue;
				}

				AActor* Actor = PriorityActors[k]->ActorInfo->Actor;

				UActorChannel* Channel = PriorityActors[k]->Channel;

				if (Channel != NULL && Time - Channel->RelevantTime <= 1.f)
				{
					PriorityActors[k]->ActorInfo->bPendingNetUpdate = true;
				}
				else if (IsActorRelevantToConnection(Actor, ConnectionViewers))
				{
					PriorityActors[k]->ActorInfo->bPendingNetUpdate = true;
					if (Channel != NULL)
					{
						Channel->RelevantTime = Time + 0.5f * FMath::SRand();
					}
				}
			}

			ConnectionViewers.Reset();
		}
	}

	if (NumClientsToTick < ClientConnections.Num())
	{
		int32 NumConnectionsToMove = NumClientsToTick;
		while (NumConnectionsToMove > 0)
		{
			UNetConnection* Connection = ClientConnections[0];
			ClientConnections.RemoveAt(0, 1);
			ClientConnections.Add(Connection);
			NumConnectionsToMove--;
		}
	}

	return Updated;
}

void NetDriver::TickFlush(UNetDriver* NetDriver, float DeltaSeconds)
{
	if (UReplicationDriver* ReplicationDriver = NetDriver->ReplicationDriver)
	{
		ReplicationDriver->ServerReplicateActors();
	}
	else
	{
		NetDriver->ServerReplicateActors(DeltaSeconds);
	}

	Originals::TickFlush(NetDriver, DeltaSeconds);
}

void NetDriver::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x27D6330, TickFlush, (void**)&Originals::TickFlush);

	Utils::Patch<uint8_t>(InSDKUtils::GetImageBase() + 0x2803FE0, 0xC3); // FNetworkObjectList::Remove crash cuz im gay?
}