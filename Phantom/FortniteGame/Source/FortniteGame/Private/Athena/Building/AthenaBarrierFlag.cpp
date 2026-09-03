#include "pch.h"
#include "FortniteGame/Public/Athena/Building/AthenaBarrierFlag.h"

void AAthenaBarrierFlag::SetFoodTeam(EBarrierFoodTeam NewFoodTeam)
{
    if (Role == ENetRole::ROLE_Authority)
    {
        FoodTeam = NewFoodTeam;
        ForceNetUpdate();
    }
}

void AAthenaBarrierFlag::SetCurrentState(EBarrierFlagState NewState)
{
    if (Role == ENetRole::ROLE_Authority && NewState != CurrentState)
        CurrentState = NewState;
}

AAthenaBarrierObjective* AthenaBarrierFlag::GetObjectiveActor(AAthenaBarrierFlag* _this, FFrame& Stack, AAthenaBarrierObjective** Ret)
{
    Stack.IncrementCode();

    UChildActorComponent* ChildActorComponent = Cast<UChildActorComponent>(_this->GetComponentByClass(UChildActorComponent::StaticClass()));

    if (ChildActorComponent == NULL || ChildActorComponent->ChildActor == NULL)
        return *Ret = NULL;

    return *Ret = Cast<AAthenaBarrierObjective>(ChildActorComponent->ChildActor);
}

void AthenaBarrierFlag::Setup()
{
    Utils::Exec(TEXT("/Script/FortniteGame.AthenaBarrierFlag.GetObjectiveActor"), GetObjectiveActor);
}