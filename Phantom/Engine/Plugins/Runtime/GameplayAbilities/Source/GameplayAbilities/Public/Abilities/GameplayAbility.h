#pragma once
#include "pch.h"
#include "Utils.h"

class GameplayAbility
{
private:
	static bool CanActivateAbility(UGameplayAbility* UGameplayAbility, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags);
public:
	static void Setup();
};