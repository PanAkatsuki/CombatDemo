// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_LightAttackBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRageStateLightAttackEndDelegate);

class UAnimMontage;
class ACombatProjectileBase;

USTRUCT(BlueprintType)
struct FLightAttackTagSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponHitSoundGameplayCueTag;
};

USTRUCT(BlueprintType)
struct FLightAttackEffectSet
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
class COMBAT_API UHeroAbility_LightAttackBase : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_LightAttackBase();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FLightAttackTagSet TagSet;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FLightAttackEffectSet EffectSet;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TSubclassOf<ACombatProjectileBase> ProjectileClass;

	int32 CurrentAttackComboCount = 1;
	int32 UsedAttackComboCount = 1; // For Handle Damage

	// Timer
	FTimerHandle AbilityEndTimerHandle;
	FTimerDelegate AbilityEndTimerDelegate;

	// Delegate
	FOnRageStateLightAttackEndDelegate OnRageStateLightAttackEndDelegate;

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

	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	// Timer
	void OnAbilityEndTimerFinished();

	// Delegate
	UFUNCTION()
	void OnRageStateAttackEnd();

	UFUNCTION()
	void OnSpawnProjectileEventReceived(FGameplayEventData InEventData);

	void ResetCurrentAttackComboCount();
};
