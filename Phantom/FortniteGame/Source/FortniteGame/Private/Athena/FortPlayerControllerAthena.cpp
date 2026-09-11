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

void FortPlayerControllerAthena::ServerEmote_Implementation(AFortPlayerControllerAthena* FortPlayerControllerAthena, const FName* AssetName)
{
	static void(*ServerEmote)(AFortPlayerController*, const FName*) = decltype(ServerEmote)(AFortPlayerController::GetDefaultObj()->VTable[0xDD0 / 8]);
	ServerEmote(FortPlayerControllerAthena, AssetName);
}

void FortPlayerControllerAthena::RemoveAllInventoryItems(AFortPlayerControllerAthena* FortPlayerControllerAthena)
{
	AFortInventory* WorldInventory = FortPlayerControllerAthena->GetWorldInventory();

	if (WorldInventory == NULL)
		return;

	WorldInventory->Inventory.ReplicatedEntries.Free();
	WorldInventory->Inventory.ItemInstances.Free();
	WorldInventory->Inventory.MarkArrayDirty();
}

void FortPlayerControllerAthena::OnPawnDied(AFortPlayerControllerAthena* FortPlayerControllerAthena, AFortPlayerPawn* KilledPawn, const FGameplayTagContainer* InTags, const FGameplayEffectContextHandle* EffectContext, AController* EventInstigator, AActor* DamageCauser, AController* DBNOFinisher)
{
	Originals::OnPawnDied(FortPlayerControllerAthena, KilledPawn, InTags, EffectContext, EventInstigator, DamageCauser, DBNOFinisher);

	AFortPlayerPawnAthena* FortPlayerPawnAthena = Cast<AFortPlayerPawnAthena>(FortPlayerControllerAthena->MyFortPawn);

	if (FortPlayerPawnAthena == NULL)
		return;

	AFortPlayerStateAthena* FortPlayerStateAthena = Cast<AFortPlayerStateAthena>(FortPlayerControllerAthena->PlayerState);

	if (FortPlayerStateAthena == NULL || InTags == NULL)
		return;

	FortPlayerStateAthena->DeathInfo.DeathCause = AFortPlayerStateAthena::ToDeathCause(*InTags, FortPlayerPawnAthena->bIsDBNO);

	AFortPlayerControllerAthena* FortPlayerControllerAthenaKiller = Cast<AFortPlayerControllerAthena>(EventInstigator);

	if (DBNOFinisher != NULL)
		FortPlayerControllerAthenaKiller = Cast<AFortPlayerControllerAthena>(DBNOFinisher);

	if (FortPlayerControllerAthenaKiller != NULL)
	{
		if (AFortPlayerPawnAthena* FortPlayerPawnAthenaKiller = Cast<AFortPlayerPawnAthena>(FortPlayerControllerAthenaKiller->Pawn))
		{
			AFortPlayerStateAthena* FortPlayerStateAthenaKiller = Cast<AFortPlayerStateAthena>(FortPlayerControllerAthenaKiller->PlayerState);

			if (FortPlayerStateAthenaKiller != NULL)
				FortPlayerStateAthena->DeathInfo.Distance = FortPlayerPawnAthenaKiller->GetDistanceTo(FortPlayerPawnAthena);
		}
	}

	if (EventInstigator != NULL)
	{
		AFortPlayerStateAthena* FortPlayerStateAthenaKiller = Cast<AFortPlayerStateAthena>(EventInstigator->PlayerState);

		if (DBNOFinisher != NULL)
			FortPlayerStateAthenaKiller = Cast<AFortPlayerStateAthena>(DBNOFinisher->PlayerState);

		FortPlayerStateAthena->DeathInfo.FinisherOrDowner = FortPlayerStateAthenaKiller;
		FortPlayerStateAthena->DeathInfo.bDBNO = false;

		AFortPlayerStateAthena* FortPlayerStateAthenaEventInstigator = Cast<AFortPlayerStateAthena>(EventInstigator->PlayerState);

		if (FortPlayerStateAthenaEventInstigator != NULL && FortPlayerStateAthenaEventInstigator != FortPlayerStateAthena)
		{
			FortPlayerStateAthenaEventInstigator->KillScore++;
			FortPlayerStateAthenaEventInstigator->OnRep_Kills();

			FortPlayerStateAthenaEventInstigator->ClientReportKill(FortPlayerStateAthena);
		}
	}
	else
	{
		FortPlayerStateAthena->DeathInfo.FinisherOrDowner = FortPlayerStateAthena;
	}

	bool IsRespawningAllowed = GGameState->IsRespawningAllowed(FortPlayerStateAthena);

	FortPlayerPawnAthena->InitializeDeathInfoOnPawnDeath(InTags, EventInstigator, DBNOFinisher);

	if (FortPlayerControllerAthena->Role == ENetRole::ROLE_Authority)
	{
		// there should be a if statement here but im to lazy to figure it out

		if (!IsRespawningAllowed)
		{
			if (EventInstigator != NULL)
			{
				FortPlayerControllerAthena->PlayerToSpectateOnDeath = EventInstigator->Pawn;

				if (FortPlayerPawnAthena != NULL)
					FortPlayerPawnAthena->KillerForSpectatorRotation = EventInstigator->Pawn;
			}

			FortPlayerControllerAthena->SpectateOnDeath();
		}
	}

	if (!IsRespawningAllowed)
	{
		if (GGameState->GamePhase > EAthenaGamePhase::Warmup && FortPlayerControllerAthena->bMarkedAlive)
		{
			FortPlayerStateAthena->SecondsAlive = FortPlayerStateAthena->SomethingAboutSecondsAliveFuncICantFindName();
			FortPlayerStateAthena->OutlivedCount = GGameState->TotalPlayers - GGameState->PlayersLeft;

			FortPlayerControllerAthena->SetMatchPlacement(GGameState->TeamsLeft);

			if (EventInstigator != NULL)
			{
				AFortPlayerControllerAthena* FortPlayerControllerAthenaEventInstigator = Cast<AFortPlayerControllerAthena>(EventInstigator);

				if (FortPlayerControllerAthenaEventInstigator != NULL)
				{
					AFortPlayerStateAthena* FortPlayerStateAthenaEventInstigator = Cast<AFortPlayerStateAthena>(FortPlayerControllerAthenaEventInstigator->PlayerState);

					if (FortPlayerStateAthenaEventInstigator != NULL)
					{
						GGameMode->RemoveFromAlivePlayers(FortPlayerControllerAthena, FortPlayerStateAthenaEventInstigator == FortPlayerStateAthena ? NULL : FortPlayerStateAthenaEventInstigator, FortPlayerControllerAthenaEventInstigator->Pawn, NULL, FortPlayerStateAthena->DeathInfo.DeathCause, false);

						if (GGameState->TeamsLeft <= 1)
							GGameMode->StartEndGamePhaseTeam((int)FortPlayerStateAthenaEventInstigator->TeamIndex, FortPlayerStateAthenaEventInstigator, 1, FortPlayerControllerAthenaEventInstigator->Pawn, NULL, FortPlayerStateAthena->DeathInfo.DeathCause);

					/*	UFortAnalytics::FireEvent_PlayerDeath(FortPlayerControllerAthena, 0, NULL, FortPlayerStateAthenaEventInstigator->GetPlayerName(), NULL, 0, 0.f, NULL);	*/
					}
				}
			}
		}
	}
}

void FortPlayerControllerAthena::Setup()
{
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x7E8 / 8, ServerRestartPlayer_Implementation);
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0xDD0 / 8, ServerEmote_Implementation);
	Utils::Virtual(AFortPlayerControllerAthena::GetDefaultObj()->VTable, 0x1270 / 8, ServerReturnToMainMenu_Implementation);

	Utils::Hook(InSDKUtils::GetImageBase() + 0xCE1AB0, OnPawnDied, (void**)&Originals::OnPawnDied);
	Utils::Rel32(InSDKUtils::GetImageBase() + 0xCBAC01, RemoveAllInventoryItems);
}