#include "pch.h"
#include "FortniteGame/Public/Athena/FortAthenaVehicle.h"

void FortAthenaVehicle::ServerUpdatePhysicsParams(AFortAthenaVehicle* FortAthenaVehicle, FReplicatedAthenaVehiclePhysicsState& InState)
{
	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(FortAthenaVehicle->RootComponent))
	{
		FTransform Transform = UKismetMathLibrary::MakeTransform(InState.Translation, InState.Rotation.Rotator(), FVector(1, 1, 1));

		PrimitiveComponent->K2_SetWorldTransform(Transform, false, NULL, true);
		PrimitiveComponent->SetPhysicsLinearVelocity(InState.LinearVelocity, 0, FName());
		PrimitiveComponent->SetPhysicsAngularVelocityInRadians(InState.AngularVelocity, 0, FName());
	}
}

void FortAthenaVehicle::Setup()
{
	Utils::Virtual(AFortAthenaVehicle::GetDefaultObj(), 0x738 / 8, ServerUpdatePhysicsParams);
}