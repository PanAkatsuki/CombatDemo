// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_Block.generated.h"


USTRUCT(BlueprintType)
struct FBlockTagSet
{
	GENERATED_BODY()

public:
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

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FBlockTagSet TagSet;
	
private:
	float BlockActivateTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability", meta = (AllowPrivateAccess = "true"))
	float PerfectBlockThreshold = 0.2f;

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

	// LantenInfo
	UFUNCTION()
	void OnDelayFinished();

	// Timer
	void OnResetCounterAttackTimerFinished();

	void ResetGlobalTimeDilation();
};
