#include "pch.h"
#include "FortniteGame/Public/Athena/FortPlayerControllerAthena.h"

void FortPlayerControllerAthena::ServerRestartPlayer_Implementation(AFortPlayerControllerAthena* FortPlayerControllerAthena)
{
	static void(*ServerRestartPlayer)(AFortPlayerControllerZone*) = decltype(ServerRestartPlayer)(AFortPlayerControllerZone::GetDefaultObj()->VTable[0x7E8 / 8]);
	ServerRestartPlayer(FortPlayerControllerAthena);
}

void FortPlayerControllerAthena::ServerReturnToMainMenu_Implementation(AFortPlayerControllerAthena* FortPlayerControllerAthena)
{
	static void(*ServerReturnToMainMenu)(AFortPlayerControllerZone*) = decltype(ServerReturnToMainMenu)(AFortPlayerControllerZone::GetDefaultObj()->VTable[0x1270 / 8]);
	ServerReturnToMainMenu(FortPlayerControllerAthena);
}

void FortPlayerControllerAthena::RemoveAllInventoryItems(AFortPlayerControllerAthena* FortPlayerControllerAthena)
{
	if (AFortInventory* WorldInventory = FortPlayerControllerAthena->GetWorldInventory())
	{
		WorldInventory->Inventory.ReplicatedEntries.Free();
		WorldInventory->Inventory.ItemInstances.Free();

		WorldInventory->Inventory.MarkArrayDirty();
	}
}

// TODO: teams on win/death

void FortPlayerControllerAthena::OnPawnDied(AFortPlayerControllerAthena* FortPlayerControllerAthena, float Damage, const FGameplayTagContainer* InTags, const FGameplayEffectContextHandle* EffectContext, AController* EventInstigator, AActor* DamageCauser)
{
	Originals::OnPawnDied(FortPlayerControllerAthena, Damage, InTags, EffectContext, EventInstigator, DamageCauser);

	if (FortPlayerControllerAthena == NULL || InTags == NULL)
		return;

	if (EventInstigator == NULL)
		EventInstigator = FortPlayerControllerAthena;

	AFortPlayerStateAthena* FortPlayerStateAthena = Cast<AFortPlayerStateAthena>(FortPlayerControllerAthena->PlayerState);

	if (FortPlayerStateAthena == NULL)
		return;

	AFortPlayerPawn* FortPlayerPawn = FortPlayerControllerAthena != NULL ? FortPlayerControllerAthena->MyFortPawn : Cast<AFortPlayerPawn>(EventInstigator->Pawn);

	if (FortPlayerPawn == NULL)
		return;

	AFortPlayerStateAthena* KillerPlayerState = Cast<AFortPlayerStateAthena>(EventInstigator->PlayerState);
	AFortPlayerPawnAthena* KillerPawn = Cast<AFortPlayerPawnAthena>(EventInstigator->Pawn);
	AFortPlayerControllerAthena* KillerPlayerController = Cast<AFortPlayerControllerAthena>(EventInstigator);

	FDeathInfo DeathInfo = FDeathInfo();

	DeathInfo.bDBNO = FortPlayerPawn->IsDBNO();
	DeathInfo.bInitialized = true;
	DeathInfo.DeathCause = FortPlayerStateAthena->ToDeathCause(*InTags, DeathInfo.bDBNO);
	DeathInfo.DeathLocation = FortPlayerPawn->K2_GetActorLocation();
	DeathInfo.FinisherOrDowner = KillerPlayerState != NULL ? KillerPlayerState : FortPlayerStateAthena;
	DeathInfo.Distance = KillerPawn != NULL ? KillerPawn->GetDistanceTo(FortPlayerPawn) : 0.f;

	FortPlayerStateAthena->InitializeDeathInfo(&DeathInfo);

	if (KillerPlayerState != NULL && KillerPlayerState != FortPlayerStateAthena)
	{
		KillerPlayerState->KillScore++;
		KillerPlayerState->OnRep_Kills();

		KillerPlayerState->ClientReportKill(FortPlayerStateAthena);

		if (KillerPlayerController != NULL && KillerPlayerController->MatchReport != NULL)
		{
			KillerPlayerController->MatchReport->MatchStats.Stats[3] = KillerPlayerState->KillScore;
			KillerPlayerController->MatchReport->MatchStats.Stats[4] = KillerPlayerState->TeamKillScore;

			if (FortPlayerPawn->IsDBNO())
			{
				KillerPlayerState->DownScore++;
				KillerPlayerState->OnRep_Downs();

				KillerPlayerController->MatchReport->MatchStats.Stats[5] = KillerPlayerState->DownScore; // GameplayStat.Profile.Match.Downs
			}
		}
	}

	if (!GGameState->IsRespawningAllowed(FortPlayerStateAthena))
	{
		FortPlayerStateAthena->Place = GGameState->PlayersLeft;
		FortPlayerStateAthena->OnRep_Place();

		FAthenaMatchTeamStats TeamStats = FAthenaMatchTeamStats();

		TeamStats.Place = FortPlayerStateAthena->Place;
		TeamStats.TotalPlayers = GGameState->TotalPlayers;

		if (FortPlayerControllerAthena->MatchReport != NULL)
		{
/*			FortPlayerControllerAthena->MatchReport->MatchStats.Stats[1] = FortPlayerStateAthena->Place; // GameplayStat.Profile.Match.Placement
			FortPlayerControllerAthena->MatchReport->MatchStats.Stats[2] = FortPlayerStateAthena->SecondsAlive; // GameplayStat.Profile.Match.SecondsAlive
			FortPlayerControllerAthena->MatchReport->MatchStats.Stats[6] = 0; // GameplayStat.Profile.Match.Assists
			FortPlayerControllerAthena->MatchReport->MatchStats.Stats[7] = 999; // GameplayStat.Profile.Match.Revives
			FortPlayerControllerAthena->MatchReport->MatchStats.Stats[8] = 1000; // GameplayStat.Profile.Match.DamageTaken
			FortPlayerControllerAthena->MatchReport->MatchStats.Stats[9] = 10; // GameplayStat.Profile.Match.TravelDistanceGround

			FortPlayerControllerAthena->MatchReport->MatchStats.MatchID = GGameState->GameSessionId;
			FortPlayerControllerAthena->MatchReport->MatchStats.MatchPlatform = FortPlayerStateAthena->Platform;*/

			FortPlayerControllerAthena->ClientSendMatchStatsForPlayer(FortPlayerControllerAthena->MatchReport->MatchStats);
		}

		FortPlayerControllerAthena->ClientSendTeamStatsForPlayer(TeamStats);
		FortPlayerControllerAthena->ClientSendEndBattleRoyaleMatchForPlayer(true, FAthenaRewardResult());

		if (GGameState->PlayersLeft == 1 && KillerPlayerState != NULL && KillerPawn != NULL && KillerPlayerController != NULL)
		{
			KillerPlayerState->Place = 1;
			KillerPlayerState->OnRep_Place();

			FAthenaMatchTeamStats KillerTeamStats = FAthenaMatchTeamStats();

			KillerTeamStats.Place = 1;
			KillerTeamStats.TotalPlayers = GGameState->TotalPlayers;

			if (KillerPlayerController->MatchReport != NULL)
			{
/*				KillerPlayerController->MatchReport->MatchStats.Stats[1] = KillerPlayerState->Place; // GameplayStat.Profile.Match.Placement
				KillerPlayerController->MatchReport->MatchStats.Stats[2] = KillerPlayerState->SecondsAlive; // GameplayStat.Profile.Match.SecondsAlive
				KillerPlayerController->MatchReport->MatchStats.Stats[3] = KillerPlayerState->KillScore;
				KillerPlayerController->MatchReport->MatchStats.Stats[4] = KillerPlayerState->TeamKillScore;
				KillerPlayerController->MatchReport->MatchStats.Stats[6] = 0; // GameplayStat.Profile.Match.Assists
				KillerPlayerController->MatchReport->MatchStats.Stats[7] = 999; // GameplayStat.Profile.Match.Revives
				KillerPlayerController->MatchReport->MatchStats.Stats[8] = 1000; // GameplayStat.Profile.Match.DamageTaken
				KillerPlayerController->MatchReport->MatchStats.Stats[9] = 10; // GameplayStat.Profile.Match.TravelDistanceGround*/

				KillerPlayerController->MatchReport->MatchStats.MatchID = GGameState->GameSessionId;
				KillerPlayerController->MatchReport->MatchStats.MatchPlatform = KillerPlayerState->Platform;

				KillerPlayerController->ClientSendMatchStatsForPlayer(KillerPlayerController->MatchReport->MatchStats);
			}

			KillerPlayerController->ClientSendTeamStatsForPlayer(KillerTeamStats);
			KillerPlayerController->ClientSendEndBattleRoyaleMatchForPlayer(true, FAthenaRewardResult());
		}
	}
}

void FortPlayerControllerAthena::Setup()
{
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x7E8 / 8, ServerRestartPlayer_Implementation);
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x1270 / 8, ServerReturnToMainMenu_Implementation);

	//Utils::Hook(InSDKUtils::GetImageBase() + 0xCE1AB0, OnPawnDied, (void**)&Originals::OnPawnDied);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xCBAC01, RemoveAllInventoryItems);
}