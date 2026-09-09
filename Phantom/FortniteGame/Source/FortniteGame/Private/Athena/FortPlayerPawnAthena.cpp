#include "pch.h"
#include "Utils.h"

// dbno related things are scuffed for now

void AFortPlayerPawnAthena::InitializeDeathInfoOnPawnDeath(const FGameplayTagContainer* InTags, AController* EventInstigator, AController* DBNOFinisher)
{
	AFortPlayerStateAthena* FortPlayerStateAthena = Cast<AFortPlayerStateAthena>(PlayerState);

	if (FortPlayerStateAthena == NULL)
		return;

	AFortPlayerControllerAthena* EventInstigatorAthena = Cast<AFortPlayerControllerAthena>(EventInstigator);

	if (EventInstigatorAthena == NULL)
		return;

	if (!bWasDBNOOnDeath && IsDBNO())
		bWasDBNOOnDeath = true;

	FDeathInfo DeathInfo = FDeathInfo{};

	DeathInfo.bDBNO = IsDBNO();
	DeathInfo.bInitialized = true;
	DeathInfo.DeathCause = AFortPlayerStateAthena::ToDeathCause(*InTags, bWasDBNOOnDeath);
	DeathInfo.DeathLocation = K2_GetActorLocation();
	DeathInfo.Distance = EventInstigatorAthena->Pawn ? GetDistanceTo(EventInstigatorAthena->Pawn) : 0.0f;
	DeathInfo.FinisherOrDowner = EventInstigator;

	FortPlayerStateAthena->DeathInfo = DeathInfo;
	FortPlayerStateAthena->OnRep_DeathInfo();
}