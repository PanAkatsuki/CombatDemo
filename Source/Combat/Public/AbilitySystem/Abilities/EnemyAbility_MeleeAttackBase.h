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

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FScalableFloat InDamageScalableFloat;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	EToggleDamageType AttackType;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	float UnblockableAttackWarningSpawnOffset = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FEnemyMeleeAttackTagSet TagSet; 

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TSubclassOf<UGameplayEffect> Effect;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void CheckIfShouldPlayUnblockableAttackWarning();

	// Callback Function For Play Montage Task 
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	// Callback Function For Wait Event Task
	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

};
