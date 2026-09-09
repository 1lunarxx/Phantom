#include "pch.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Barrier.h"

void AFortAthenaMutator_Barrier::SetupTeamStates()
{
    if (CachedGameState != NULL)
    {
        UFortPlaylistAthena* CurrentPlaylistData = CachedGameState->GetCurrentPlaylistData();

        if (CurrentPlaylistData != NULL)
        {
            Team_0_State.FoodTeam = EBarrierFoodTeam::Burger;
            Team_0_State.TeamNum = CurrentPlaylistData->DefaultFirstTeam;
            Team_1_State.FoodTeam = EBarrierFoodTeam::Tomato;
            Team_1_State.TeamNum = CurrentPlaylistData->DefaultLastTeam;
        }
    }
}

void AFortAthenaMutator_Barrier::SpawnBarrier(const FVector* WallStart, const FVector* WallEnd)
{
    BigBaseWall = AAthenaBigBaseWall::CreateWall(GWorld, BigBaseWallClass.Get(), WallStart, WallEnd, -2500.f);
}

void AFortAthenaMutator_Barrier::SpawnModeObjectives()
{
    float SafeZonePhaseWhenToBringDownWall = UFortScalableFloatUtils::GetValueAtLevel(this->SafeZonePhaseWhenToBringDownWall, 0.f);
    float ObjectiveDistanceFromWall = UFortScalableFloatUtils::GetValueAtLevel(this->ObjectiveDistanceFromWall, 0.f);

    FVector Team_0_Location = GGameMode->SafeZoneLocations[(int)SafeZonePhaseWhenToBringDownWall] + (BigBaseWall->GetActorRightVector() * ObjectiveDistanceFromWall);
    FVector Team_1_Location = GGameMode->SafeZoneLocations[(int)SafeZonePhaseWhenToBringDownWall] - (BigBaseWall->GetActorRightVector() * ObjectiveDistanceFromWall);

    Team_0_Location.Z = 6500.f;
    Team_1_Location.Z = 6500.f;

    FRotator Team_0_Rotation = UKismetMathLibrary::Conv_VectorToRotator(Team_1_Location - Team_0_Location);
    FRotator Team_1_Rotation = UKismetMathLibrary::Conv_VectorToRotator(Team_0_Location - Team_1_Location);

    SetupTeamObjective(EBarrierFoodTeam::Burger, Team_0_Location, Team_0_Rotation);
    SetupTeamObjective(EBarrierFoodTeam::Tomato, Team_1_Location, Team_1_Rotation);
}

void AFortAthenaMutator_Barrier::SetupTeamObjective(EBarrierFoodTeam FoodTeam, FVector Location, FRotator Rotation)
{
    FBarrierTeamState* TeamState = &Team_0_State;

    if (FoodTeam == EBarrierFoodTeam::Tomato)
        TeamState = &Team_1_State;

    AAthenaBarrierFlag* BarrierFlag = SpawnObjectiveActor(ObjectiveFlag.Get(), &Location, &Rotation);

    if (BarrierFlag == NULL)
        return;

    BarrierFlag->SetTeam(TeamState->TeamNum);
    BarrierFlag->SetFoodTeam(TeamState->FoodTeam);
    BarrierFlag->SetCurrentState(EBarrierFlagState::FlagUp);

    TeamState->ObjectiveFlag = BarrierFlag;
    TeamState->bRespawnEnabled = true;

    FGameplayMutatorObjectData MutatorObjectData = FGameplayMutatorObjectData{};

    MutatorObjectData.ReplicationID = -1;
    MutatorObjectData.ReplicationKey = -1;
    MutatorObjectData.MostRecentArrayReplicationKey = -1;
    MutatorObjectData.TheObject = BarrierFlag;

    CachedGameState->MutatorObjectDataArray.MarkItemDirty(MutatorObjectData);
    CachedGameState->MutatorObjectDataArray.ObjectDataList.Add(MutatorObjectData);

    AAthenaBarrierObjective* ObjectiveActor = BarrierFlag->GetObjectiveActor();

    if (ObjectiveActor == NULL)
        return;

    ObjectiveActor->SetTeam(TeamState->TeamNum);
    ObjectiveActor->SetFoodTeam(TeamState->FoodTeam);
    ObjectiveActor->SetAllowDamage(true);

    TeamState->ObjectiveObject = ObjectiveActor;
}

AAthenaBarrierFlag* AFortAthenaMutator_Barrier::SpawnObjectiveActor(TSubclassOf<AAthenaBarrierFlag> InActorClass, const FVector* InActorLocation, const FRotator* InActorRotation)
{
    return GWorld->SpawnActor<AAthenaBarrierFlag>(*InActorLocation, *InActorRotation, InActorClass.Get());
}

void AFortAthenaMutator_Barrier::OnObjectiveDestroyed(AAthenaBarrierObjective* Objective)
{
    if (Objective != NULL)
    {
        FBarrierTeamState* TeamState = &Team_0_State;

        if (TeamState->TeamNum != Objective->Team)
            TeamState = &Team_1_State;

        WinningTeam = Team_0_State.TeamNum;

        if (WinningTeam == Objective->Team)
            WinningTeam = Team_1_State.TeamNum;

        if (Team_0_State.ObjectiveObject != NULL)
            Team_0_State.ObjectiveObject->SetAllowDamage(false);

        if (Team_1_State.ObjectiveObject != NULL)
            Team_1_State.ObjectiveObject->SetAllowDamage(false);

        if (TeamState->ObjectiveFlag != NULL)
            TeamState->ObjectiveFlag->SetCurrentState(EBarrierFlagState::FlagDown);
    }
}

void FortAthenaMutator_Barrier::BeginPlay(AFortAthenaMutator_Barrier* FortAthenaMutator_Barrier)
{
    Originals::BeginPlay(FortAthenaMutator_Barrier);

    FAircraftFlightInfo& FlightPathMidLine = GGameState->FlightPathMidLine;

    FVector FlightMidLineEnd = FlightPathMidLine.GetFlightEnd();
    FVector FlightStartLocation = FlightPathMidLine.FlightStartLocation;

    FortAthenaMutator_Barrier->SpawnBarrier(&FlightStartLocation, &FlightMidLineEnd);
    FortAthenaMutator_Barrier->SetupTeamStates();
}

void FortAthenaMutator_Barrier::OnGamePhaseStepChanged(AFortAthenaMutator_Barrier* FortAthenaMutator_Barrier, EAthenaGamePhaseStep GamePhaseStep)
{
    Originals::OnGamePhaseStepChanged(FortAthenaMutator_Barrier, GamePhaseStep);

    if (GamePhaseStep == EAthenaGamePhaseStep::BusLocked)
        FortAthenaMutator_Barrier->SpawnModeObjectives();
}

void FortAthenaMutator_Barrier::Setup()
{
    Utils::Hook(InSDKUtils::GetImageBase() + 0xD2E760, BeginPlay, (void**)&Originals::BeginPlay);
    Utils::Hook(InSDKUtils::GetImageBase() + 0xD46DC0, OnGamePhaseStepChanged, (void**)&Originals::OnGamePhaseStepChanged);
}