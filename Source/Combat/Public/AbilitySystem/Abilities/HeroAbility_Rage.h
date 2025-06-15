// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_Rage.generated.h"


USTRUCT(BlueprintType)
struct FRageTagSet
{
	GENERATED_BODY()

public:
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

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FRageTagSet TagSet;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TSubclassOf<UGameplayEffect> RageCostEffectClass;

	// Effect Handle
	FActiveGameplayEffectHandle AppliedRageCostGameplayEffectHandle;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	// Set Wait Tag Add
	void SetWaitGameplayTagAdd();

	UFUNCTION()
	void OnTagAdded();
};
