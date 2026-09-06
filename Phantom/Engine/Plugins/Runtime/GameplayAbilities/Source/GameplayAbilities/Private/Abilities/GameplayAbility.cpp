#include "pch.h"
#include "GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h"

bool GameplayAbility::CanActivateAbility(UGameplayAbility* UGameplayAbility, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags)
{
	return true;
}

void GameplayAbility::Setup()
{
	Utils::Hook(InSDKUtils::GetImageBase() + 0x656320, CanActivateAbility);
}