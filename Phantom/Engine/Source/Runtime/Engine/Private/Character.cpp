#include "pch.h"
#include "GameFramework/Character.h"

void Character::ClientCheatFly_Implementation(ACharacter* Character)
{
	Character->SetActorEnableCollision(true);

	if (UCharacterMovementComponent* CharacterMovement = Character->CharacterMovement)
	{
		CharacterMovement->bCheatFlying = true;
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Flying, 0);
	}
}

void Character::ClientCheatGhost_Implementation(ACharacter* Character)
{
	Character->SetActorEnableCollision(false);

	if (UCharacterMovementComponent* CharacterMovement = Character->CharacterMovement)
	{
		CharacterMovement->bCheatFlying = true;
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Flying, 0);
	}
}

void Character::ClientCheatWalk_Implementation(ACharacter* Character)
{
	Character->SetActorEnableCollision(true);

	if (UCharacterMovementComponent* CharacterMovement = Character->CharacterMovement)
	{
		CharacterMovement->bCheatFlying = false;
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Falling, 0);
	}
}

void Character::Setup()
{
	Utils::Virtual(ACharacter::GetDefaultObj(), 0x820 / 8, ClientCheatFly_Implementation);
	Utils::Virtual(ACharacter::GetDefaultObj(), 0x828 / 8, ClientCheatGhost_Implementation);
	Utils::Virtual(ACharacter::GetDefaultObj(), 0x818 / 8, ClientCheatWalk_Implementation);
}