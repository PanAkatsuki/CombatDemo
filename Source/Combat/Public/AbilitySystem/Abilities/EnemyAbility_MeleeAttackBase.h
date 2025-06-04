// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatEnemyGameplayAbility.h"

#include "Components/Fight/PawnFightComponent.h"

#include "EnemyAbility_MeleeAttackBase.generated.h"



USTRUCT(BlueprintType)
struct FEnemyMeleeAttackTagSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag UnBlockableWarningGameplayCueTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponHitSoundGameplayCueTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WaitMontageEventTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TargetHitReactEventTag;
};

/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyAbility_MeleeAttackBase : public UCombatEnemyGameplayAbility
{
	GENERATED_BODY()
	
public:
	UEnemyAbility_MeleeAttackBase();

protected:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat InDamageScalableFloat;

	UPROPERTY(EditDefaultsOnly)
	EToggleDamageType AttackType;

	UPROPERTY(EditDefaultsOnly)
	float UnblockableAttackWarningSpawnOffset = 100.f;

	UPROPERTY(EditDefaultsOnly)
	FEnemyMeleeAttackTagSet TagSet; 

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> Effect;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	void CheckIfShouldPlayUnblockableAttackWarning();

	// Montage task
	void SetPlayMontageTask(UAnimMontage* InMontageToPlay);

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	//Wait event task
	void SetWaitMontageEventTask(FGameplayTag& InWaitMontageEventTag);

	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	void ExecuteGameplayCueOnOnwer(FGameplayTag& InGameplayCueTag) const;
	void HandleDamage(FGameplayEventData& InEventData, FGameplayEffectSpecHandle& InGameplayEffectSpecHandle, FEnemyMeleeAttackTagSet& InTagSet);
};
