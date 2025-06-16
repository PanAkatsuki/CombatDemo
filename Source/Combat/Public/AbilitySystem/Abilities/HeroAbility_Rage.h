// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_Rage.generated.h"


USTRUCT(BlueprintType)
struct FRageTagSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RageGameplayCueTag;
};

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_Rage : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_Rage();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FRageTagSet TagSet;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TSubclassOf<UGameplayEffect> RageCostEffectClass;

	// Effect Handle
	FActiveGameplayEffectHandle AppliedRageCostGameplayEffectHandle;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	// Callback funtion for play montage task
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	// Callback funtion for wait event task
	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	// Callback funtion for Wait Tag Added Task
	UFUNCTION()
	void OnTagAdded();
};
