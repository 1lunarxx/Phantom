#include "pch.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Barrier.h"

void AFortAthenaMutator_Barrier::SetupTeamStates()
{
    if (CachedGameState != NULL)
    {
        UFortPlaylistAthena* CurrentPlaylistData = CachedGameState->CurrentPlaylistInfo.BasePlaylist;

        if (CurrentPlaylistData != NULL)
        {
            Team_0_State.TeamNum = CurrentPlaylistData->DefaultFirstTeam;
            Team_1_State.FoodTeam = EBarrierFoodTeam::Tomato;
            Team_1_State.TeamNum = (EFortTeam)((int)Team_0_State.TeamNum + 1);
        }
    }
}

void AFortAthenaMutator_Barrier::SpawnBarrier(const FVector* WallStart, const FVector* WallEnd)
{
    BigBaseWall = AAthenaBigBaseWall::CreateWall(UWorld::GetWorld(), BigBaseWallClass.Get(), WallStart, WallEnd, -2500.f);
}

void AFortAthenaMutator_Barrier::SpawnModeObjectives()
{
    float SafeZonePhaseWhenToBringDownWall = UFortScalableFloatUtils::GetValueAtLevel(this->SafeZonePhaseWhenToBringDownWall, 0.f);
    float ObjectiveDistanceFromWall = UFortScalableFloatUtils::GetValueAtLevel(this->ObjectiveDistanceFromWall, 0.f);
    float ObjectiveZOffset = UFortScalableFloatUtils::GetValueAtLevel(this->ObjectiveZOffset, 0.f);

    FVector Team_0_Location = GGameMode->SafeZoneLocations[(int)SafeZonePhaseWhenToBringDownWall] + (BigBaseWall->GetActorRightVector() * ObjectiveDistanceFromWall);
    FVector Team_1_Location = GGameMode->SafeZoneLocations[(int)SafeZonePhaseWhenToBringDownWall] - (BigBaseWall->GetActorRightVector() * ObjectiveDistanceFromWall);

    Team_0_Location.Z = 7000.f;
    Team_1_Location.Z = 7000.f;

    FRotator Team_0_Rotation = UKismetMathLibrary::Conv_VectorToRotator(Team_1_Location - Team_0_Location);
    FRotator Team_1_Rotation = UKismetMathLibrary::Conv_VectorToRotator(Team_0_Location - Team_1_Location);

    if (AAthenaBarrierFlag* BarrierFlag = SpawnObjectiveActor(ObjectiveFlag.Get(), &Team_0_Location, &Team_0_Rotation))
    {
        BarrierFlag->SetFoodTeam(Team_0_State.FoodTeam);
        BarrierFlag->SetTeam(Team_0_State.TeamNum);

        BarrierFlag->SetCurrentState(EBarrierFlagState::FlagUp);

        FActorSpawnParameters SpawnParams = FActorSpawnParameters();
        SpawnParams.SpawnCollisionHandlingOverride = 1;

        if (AAthenaBarrierObjective* ObjectiveActor = BarrierFlag->GetObjectiveActor())
        {
            ObjectiveActor->SetFoodTeam(Team_0_State.FoodTeam);
            ObjectiveActor->SetTeam(Team_0_State.TeamNum);

            ObjectiveActor->SetAllowDamage(true);
            ObjectiveActor->SetObjectiveDamageState(EBarrierObjectiveDamageState::Health_50);

            Team_0_State.ObjectiveObject = ObjectiveActor;
        }

        Team_0_State.ObjectiveFlag = BarrierFlag;
        Team_0_State.bRespawnEnabled = true;
    }

    if (AAthenaBarrierFlag* BarrierFlag = SpawnObjectiveActor(ObjectiveFlag, &Team_1_Location, &Team_1_Rotation))
    {
        BarrierFlag->SetFoodTeam(Team_1_State.FoodTeam);
        BarrierFlag->SetTeam(Team_1_State.TeamNum);

        BarrierFlag->SetCurrentState(EBarrierFlagState::FlagUp);

        FActorSpawnParameters SpawnParams = FActorSpawnParameters();
        SpawnParams.SpawnCollisionHandlingOverride = 1;

        if (AAthenaBarrierObjective* ObjectiveActor = BarrierFlag->GetObjectiveActor())
        {
            ObjectiveActor->SetFoodTeam(Team_1_State.FoodTeam);
            ObjectiveActor->SetTeam(Team_1_State.TeamNum);

            ObjectiveActor->SetAllowDamage(true);
            ObjectiveActor->SetObjectiveDamageState(EBarrierObjectiveDamageState::Health_50);

            Team_1_State.ObjectiveObject = ObjectiveActor;
        }

        Team_1_State.ObjectiveFlag = BarrierFlag;
        Team_1_State.bRespawnEnabled = true;
    }

    if (Team_0_State.ObjectiveFlag != NULL)
    {
        FGameplayMutatorObjectData ObjectData = FGameplayMutatorObjectData{};

        ObjectData.ReplicationID = -1;
        ObjectData.ReplicationKey = -1;
        ObjectData.MostRecentArrayReplicationKey = -1;
        ObjectData.TheObject = Team_0_State.ObjectiveFlag;

        CachedGameState->MutatorObjectDataArray.MarkItemDirty(ObjectData);
        CachedGameState->MutatorObjectDataArray.ObjectDataList.Add(ObjectData);
    }

    if (Team_1_State.ObjectiveFlag != NULL)
    {
        FGameplayMutatorObjectData ObjectData = FGameplayMutatorObjectData{};

        ObjectData.ReplicationID = -1;
        ObjectData.ReplicationKey = -1;
        ObjectData.MostRecentArrayReplicationKey = -1;
        ObjectData.TheObject = Team_1_State.ObjectiveFlag;

        CachedGameState->MutatorObjectDataArray.MarkItemDirty(ObjectData);
        CachedGameState->MutatorObjectDataArray.ObjectDataList.Add(ObjectData);
    }
}

AAthenaBarrierFlag* AFortAthenaMutator_Barrier::SpawnObjectiveActor(TSubclassOf<AAthenaBarrierFlag> InActorClass, const FVector* InActorLocation, const FRotator* InActorRotation)
{
    FActorSpawnParameters SpawnParams = FActorSpawnParameters();
    SpawnParams.SpawnCollisionHandlingOverride = 1;

    AAthenaBarrierFlag* ObjectiveFlag = Cast<AAthenaBarrierFlag>(UWorld::GetWorld()->SpawnActor(InActorClass, InActorLocation, InActorRotation, &SpawnParams));

    if (ObjectiveFlag != NULL)
        UGameplayStatics::FinishSpawningActor(ObjectiveFlag, UKismetMathLibrary::MakeTransform(*InActorLocation, *InActorRotation, FVector(1, 1, 1)));

    return ObjectiveFlag;
}

void AFortAthenaMutator_Barrier::OnObjectiveDestroyed(AAthenaBarrierObjective* Objective)
{
    if (Objective != NULL)
    {
        FBarrierTeamState* TeamState = &Team_0_State;

        if (TeamState->TeamNum != Objective->Team)
            TeamState = &Team_1_State;

        if (!(int)WinningTeam)
        {
            WinningTeam = Team_0_State.TeamNum;

            if (WinningTeam == Objective->Team)
                WinningTeam = Team_1_State.TeamNum;
        }

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

    FortAthenaMutator_Barrier->SpawnBarrier(&FlightPathMidLine.FlightStartLocation, &FlightMidLineEnd);
    FortAthenaMutator_Barrier->SetupTeamStates();
}

void FortAthenaMutator_Barrier::OnGamePhaseStepChanged(AFortAthenaMutator_Barrier* FortAthenaMutator_Barrier, EAthenaGamePhaseStep GamePhaseStep)
{
    if (GamePhaseStep == EAthenaGamePhaseStep::BusLocked)
        FortAthenaMutator_Barrier->SpawnModeObjectives();

    Originals::OnGamePhaseStepChanged(FortAthenaMutator_Barrier, GamePhaseStep);
}

void FortAthenaMutator_Barrier::Setup()
{
    Utils::Hook(InSDKUtils::GetImageBase() + 0xD2E760, BeginPlay, (void**)&Originals::BeginPlay);
    Utils::Hook(InSDKUtils::GetImageBase() + 0xD46DC0, OnGamePhaseStepChanged, (void**)&Originals::OnGamePhaseStepChanged);
}