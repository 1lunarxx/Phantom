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

	FortPlayerStateAthena->DeathInfo.bDBNO = IsDBNO();
	FortPlayerStateAthena->DeathInfo.bInitialized = true;
	FortPlayerStateAthena->DeathInfo.DeathLocation = K2_GetActorLocation();

	FortPlayerStateAthena->OnRep_DeathInfo();
}