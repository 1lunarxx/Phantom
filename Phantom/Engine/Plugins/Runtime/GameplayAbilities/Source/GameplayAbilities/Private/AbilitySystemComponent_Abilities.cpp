#include "pch.h"
#include "GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"

void AbilitySystemComponent::ConsumeAllReplicatedData(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey)
{
/*	FAbilityReplicatedDataCache* CachedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, AbilityOriginalPredictionKey.Current));
	if (CachedData)
	{
		CachedData->Reset();
	}*/
}

void AbilitySystemComponent::InternalServerTryActiveAbility(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey, const FGameplayEventData* TriggerEventData)
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		/*ABILITY_LOG(Display, TEXT("InternalServerTryActiveAbility. Rejecting ClientActivation of ability with invalid SpecHandle!"));*/
		AbilitySystemComponent->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	// Consume any pending target info, to clear out cancels from old executions
	ConsumeAllReplicatedData(AbilitySystemComponent, Handle, PredictionKey);

	const UGameplayAbility* AbilityToActivate = Spec->Ability;

	if (AbilityToActivate == NULL)
		return;

	UGameplayAbility* InstancedAbility = nullptr;
	Spec->InputPressed = true;

	// Attempt to activate the ability (server side) and tell the client if it succeeded or failed.
	if (AbilitySystemComponent->InternalTryActivateAbility(Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
	{
		// TryActivateAbility handles notifying the client of success
	}
	else
	{
		AbilitySystemComponent->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		Spec->InputPressed = false;
	}
	AbilitySystemComponent->MarkAbilitySpecDirty(Spec);
}

void AbilitySystemComponent::Setup()
{
	Utils::Virtual(UAbilitySystemComponent::GetDefaultObj(), 0x7A0 / 8, InternalServerTryActiveAbility);
}