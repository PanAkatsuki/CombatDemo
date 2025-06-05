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
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ShieldGameplayCueTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WaitMontageEventTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SuccessfulBlockGameplayCueTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PerfectBlockJumpToFinisherTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PerfectBlockCounterAttackTag;

	UPROPERTY(EditDefaultsOnly)
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
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditDefaultsOnly)
	FBlockTagSet TagSet;

	float BlockActivateTime = 0.f;
	float PerfectBlockThreshold = 0.2f;
	bool bIsPerfectBlock = false;

	FLatentActionInfo LatentInfo;

	// Timer
	FTimerHandle ResetCounterAttackTimerHandle;
	FTimerDelegate ResetCounterAttackTimerDelegate;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	void SetPlayMontageTask(UAnimMontage* InMontageToPlay);

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	void ResetGlobalTimeDilation();

	void SetWaitMontageEventTask(FGameplayTag& InWaitMontageEventTag);

	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	// LantenInfo
	UFUNCTION()
	void OnDelayFinished();

	void SetTimer(FTimerHandle& InTimerHandle, FTimerDelegate& InTimerDelegate);
	void OnResetCounterAttackTimerFinished();
};
