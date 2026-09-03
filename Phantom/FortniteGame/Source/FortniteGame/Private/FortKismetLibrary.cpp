#include "pch.h"
#include "FortniteGame/Public/FortKismetLibrary.h"

AFortAIGoalManager* FortKismetLibrary::GetAIGoalManager(UFortKismetLibrary* FortKismetLibrary, FFrame& Stack, AFortAIGoalManager** Ret)
{
    UObject* WorldContextObject;

    Stack.StepCompiledIn(&WorldContextObject);
    Stack.IncrementCode();

    if (AActor* WorldContext = Cast<AActor>(WorldContextObject))
    {
        if (UWorld* World = WorldContext->GetWorld())
            return *Ret = Cast<AFortGameModeZone>(World->AuthorityGameMode)->AIGoalManager;
    }

    return *Ret = GGameMode->AIGoalManager;
}

void FortKismetLibrary::Setup()
{
    Utils::Exec(TEXT("/Script/FortniteGame.FortKismetLibrary.GetAIGoalManager"), GetAIGoalManager);
}