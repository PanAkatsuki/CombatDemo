// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_HeavyAttackBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRageStateHeavyAttackEndDelegate);

class ACombatProjectileBase;

USTRUCT(BlueprintType)
struct FHeavyAttackTagSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponHitSoundGameplayCueTag;
};

USTRUCT(BlueprintType)
struct FHeavyAttackEffectSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DealDamageEffectClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GainRageEffectClass;
};

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_HeavyAttackBase : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_HeavyAttackBase();

private:
	UPROPERTY(EditDefaultsOnly)
	FHeavyAttackTagSet TagSet;

	UPROPERTY(EditDefaultsOnly)
	FHeavyAttackEffectSet EffectSet;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACombatProjectileBase> ProjectileClass;

	int32 CurrentAttackComboCount = 1;
	int32 UsedAttackComboCount = 1; // For Handle Damage

	// Timer
	FTimerHandle AbilityEndTimerHandle;
	FTimerDelegate AbilityEndTimerDelegate;

	// Delegate
	FOnRageStateHeavyAttackEndDelegate OnRageStateHeavyAttackEndDelegate;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UAnimMontage* FindMontageToPlayWithKey(TMap<int32, UAnimMontage*>& InAnimMontagesMap, int32 InKey);

	// Callback function for play montage task
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	//Callback function for wait event task
	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	// Callback function for Timer
	void OnAbilityEndTimerFinished();

	// Callback function for OnRageStateHeavyAttackEndDelegate
	UFUNCTION()
	void OnRageStateAttackEnd();

	// Callback funtion for OnRageStateHeavyAttackEndDelegate
	UFUNCTION()
	void OnSpawnProjectileEventReceived(FGameplayEventData InEventData);

	void ResetCurrentAttackComboCount();
};
