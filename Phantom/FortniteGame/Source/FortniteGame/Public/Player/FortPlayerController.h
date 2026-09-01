#pragma once
#include "pch.h"
#include "Utils.h"

class FortPlayerController
{
	class Originals
	{
	public:
		static inline void (*ServerAttemptInteract)(AFortPlayerController* FortPlayerController, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, ETInteractionType InteractType, UObject* OptionalObjectData);
	};

private:
	static void ServerExecuteInventoryItem_Implementation(AFortPlayerController* FortPlayerController, FGuid& ItemGuid);
	static void ServerAttemptInteract(AFortPlayerController* FortPlayerController, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, ETInteractionType InteractType, UObject* OptionalObjectData);
public:
	static void Setup();
};