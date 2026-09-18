// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
#include "Utils.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

FName::FName(FString String)
{
	*this = UKismetStringLibrary::Conv_StringToName(String);
}

bool AFortPlayerController::ObjectiveOncePerMatch(UFortQuestItemDefinition* FortQuestItemDefinition, TMap<UFortQuestManager*, UFortQuestItemDefinition*>* ObjectiveOncePerMatchMap)
{
	if (!FortQuestItemDefinition->bAthenaUpdateObjectiveOncePerMatch)
		return false;

	UFortQuestManager* FortQuestManager = GetQuestManager(ESubGame::Athena);

	if (FortQuestManager == NULL)
		return false;
	
	for (const auto& [QuestManager, QuestItemDef] : *ObjectiveOncePerMatchMap)
	{
		if (QuestManager == FortQuestManager && QuestItemDef == FortQuestItemDefinition)
			return true;
	}

	ObjectiveOncePerMatchMap->Add(FortQuestManager, FortQuestItemDefinition);

	return false;
}

FVector FAircraftFlightInfo::GetFlightEnd()
{
	FVector Direction = UKismetMathLibrary::Conv_RotatorToVector(FlightStartRotation);

	const double SizeSquared = Direction.X * Direction.X + Direction.Y * Direction.Y + Direction.Z * Direction.Z;
	if (SizeSquared > 1e-8)
	{
		Direction /= std::sqrt(SizeSquared);
	}
	else
	{
		Direction = FVector();
	}

	return FlightStartLocation + Direction * (FlightSpeed * TimeTillFlightEnd);
}

void FFortItemEntry::SetLoadedAmmo(int InCount)
{
	if (InCount != LoadedAmmo)
	{
		LoadedAmmo = InCount;

		if (AFortInventory* FortInventory = ParentInventory.Get())
			FortInventory->SetItemRequiresUpdate(this);

		bIsDirty = 1;
	}
}

void FFortItemEntry::SetInInventoryOverflow(bool bOverflow)
{
	if (inventory_overflow_date != bOverflow)
	{
		inventory_overflow_date = bOverflow;

		if (AFortInventory* FortInventory = ParentInventory.Get())
			FortInventory->SetItemRequiresUpdate(this);

		bIsDirty = 1;
	}
}

void FGameplayMutatorObjectData::PostReplicatedAdd(struct FGameplayMutatorObjectDataArray* InArraySerializer)
{
	InArraySerializer->MarkItemDirty(*this);
	InArraySerializer->ObjectDataList.Add(*this);
}

// replication stuff for stw

FNetViewer::FNetViewer(UNetConnection* InConnection, float DeltaSeconds) :
	Connection(InConnection),
	InViewer(InConnection->PlayerController ? InConnection->PlayerController : InConnection->OwningActor),
	ViewTarget(InConnection->ViewTarget),
	ViewLocation(FVector()),
	ViewDir(FVector())
{
	if (InConnection->OwningActor == NULL)
		return;

	APlayerController* ViewingController = InConnection->PlayerController;

	if (ViewTarget != NULL)
		ViewLocation = ViewTarget->K2_GetActorLocation();

	if (ViewingController != NULL)
	{
		FRotator ViewRotation = ViewingController->GetControlRotation();
		ViewingController->GetPlayerViewPoint(&ViewLocation, &ViewRotation);
		ViewDir = UKismetMathLibrary::Conv_RotatorToVector(ViewRotation);
	}
}

FWeakObjectPtr::FWeakObjectPtr(UObject* Object)
	: ObjectIndex(0), ObjectSerialNumber(0)
{
	if (Object == NULL)
		return;

	ObjectIndex = Object->Index;

	FUObjectItem* ObjectItem = UObject::GObjects->GetItemByIndex(ObjectIndex);

	if (ObjectItem != NULL)
		ObjectSerialNumber = ObjectItem->SerialNumber;
}

UActorChannel* UNetConnection::FindActorChannelRef(const TWeakObjectPtr<AActor>& Actor)
{
	for (UChannel* OpenChannel : OpenChannels)
	{
		if (UActorChannel* ActorChannel = Cast<UActorChannel>(OpenChannel))
		{
			if (ActorChannel->Actor == Actor.Get())
				return ActorChannel;
		}
	}

	return NULL;
}

void UNetConnection::RemoveDestructionInfo(FActorDestructionInfo* DestructionInfo)
{
	if (DestructionInfo == NULL)
		return;

	for (int32 i = 0; i < GetDestroyedStartupOrDormantActorGUIDs().Num(); i++)
	{
		const auto& DestroyedStartupOrDormantActorGUID = GetDestroyedStartupOrDormantActorGUIDs()[i];

		if (DestroyedStartupOrDormantActorGUID.ObjectId == DestructionInfo->NetGUID.ObjectId)
		{
			GetDestroyedStartupOrDormantActorGUIDs().Remove(i);
			return;
		}
	}
}

ULevel* AActor::GetLevel()
{
	static ULevel* (*GetLevel)(AActor*, UClass*) = decltype(GetLevel)(InSDKUtils::GetImageBase() + 0x19BC230);
	return GetLevel(this, ULevel::StaticClass());
}