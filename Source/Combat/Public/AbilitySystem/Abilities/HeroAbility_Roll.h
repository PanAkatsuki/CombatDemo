// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_Roll.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam()

class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_Roll : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_Roll();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FScalableFloat RollDistanceScalableFloat;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	float TimeSlowTime = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	float TimeDilation = 0.05f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FGameplayTag TimeSlowGameplayCueTag;

	FLatentActionInfo RollLatentInfo;

	// Timer
	FTimerHandle PerfectRollTimerHandle;
	FTimerDelegate PerfectRollTimerDelegate;

	// Task Handle
	UAbilityTask_WaitGameplayEvent* WaitPerfectRollTaskHandle;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	// Callback funtion for Delay, cus if run ComputeRollDiractionAndDistance after ActivateAbility immediately, 
	UFUNCTION()
	void OnDelayFinished();

	void ComputeRollDiractionAndDistance();

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

	UFUNCTION()
	void OnPerfectRollTimerEnd();
};
