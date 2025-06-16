// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_Block.generated.h"


USTRUCT(BlueprintType)
struct FBlockTagSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FGameplayTag ShieldGameplayCueTag;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FGameplayTag SuccessfulBlockGameplayCueTag;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FGameplayTag PerfectBlockGameplayCueTag;
};

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_Block : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_Block();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FBlockTagSet TagSet;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability", meta = (AllowPrivateAccess = "true"))
	float PerfectBlockThreshold = 0.2f;

	float BlockActivateTime = 0.f;
	bool bIsPerfectBlock = false;

	// LatentInfo
	FLatentActionInfo BlockLatentInfo;

	// Timer
	FTimerHandle ResetCounterAttackTimerHandle;
	FTimerDelegate ResetCounterAttackTimerDelegate;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	// Callback funciton for play montage task
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	// Callback funciton for wait event task
	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	// Callback function for LantenInfo
	UFUNCTION()
	void OnDelayFinished();

	// Callback function for Timer
	void OnResetCounterAttackTimerFinished();

	void ResetGlobalTimeDilation();
};
