#include "pch.h"
#include "FortniteGame/Public/STW/FortGameModeOutpost.h"

void FortGameModeOutpost::HandleStartingNewPlayer(AFortGameModeOutpost* FortGameModeOutpost, APlayerController* NewPlayer)
{
	Originals::HandleStartingNewPlayer(FortGameModeOutpost, NewPlayer);

	if (AFortPlayerStateOutpost* FortPlayerStateOutpost = Cast<AFortPlayerStateOutpost>(NewPlayer->PlayerState))
	{
		FortPlayerStateOutpost->SetIsWorldDataOwner(true);
	}
}

void FortGameModeOutpost::Setup()
{
	Utils::Virtual(AFortGameModeOutpost::GetDefaultObj()->VTable, 0x640 / 8, HandleStartingNewPlayer, (void**)&Originals::HandleStartingNewPlayer);
}