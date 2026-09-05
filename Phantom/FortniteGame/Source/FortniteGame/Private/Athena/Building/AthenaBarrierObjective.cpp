#include "pch.h"

void AAthenaBarrierObjective::SetFoodTeam(EBarrierFoodTeam NewFoodTeam)
{
	if (Role == ENetRole::ROLE_Authority)
	{
		FoodTeam = NewFoodTeam;

		if (NewFoodTeam != EBarrierFoodTeam::MAX)
		{
			uint8 FoodDisplayData_FoodTeam = (uint8)NewFoodTeam;

			TArray<UMaterialInterface*> MaterialsToSwap = FoodDisplayData[FoodDisplayData_FoodTeam].MaterialsToSwap;
			UStaticMesh* HeadMesh = FoodDisplayData[FoodDisplayData_FoodTeam].HeadMesh;

			if (MaterialsToSwap.IsValid() && HeadMesh != NULL)
				SetHeadMesh(HeadMesh, FVector(), FVector(), MaterialsToSwap);
		}

		OnRep_FoodTeam();
	}
}

void AAthenaBarrierObjective::SetAllowDamage(bool bNewDamageState)
{
	if (Role == ENetRole::ROLE_Authority)
		bAllowDamage = bNewDamageState;
}

void AAthenaBarrierObjective::SetObjectiveDamageState(EBarrierObjectiveDamageState NewState)
{
	if (Role == ENetRole::ROLE_Authority)
		ObjectiveDamageState = NewState;
}