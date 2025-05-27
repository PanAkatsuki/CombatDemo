// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_LightAttackBase.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FLightAttackTagSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponHitSoundGameplayCueTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WaitMontageEventTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SetByCallerAttackTypeTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TargetHitReactEventTag;
};

USTRUCT(BlueprintType)
struct FLightAttackEffectSet
{
	GENERATED_BODY()

public:
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

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, UAnimMontage*> AttackMontagesMap;

	UPROPERTY(EditDefaultsOnly)
	FLightAttackTagSet TagSet;

	UPROPERTY(EditDefaultsOnly)
	FLightAttackEffectSet EffectSet;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentAttackComboCount = 1;

	UPROPERTY(VisibleAnywhere)
	int32 UsedAttackComboCount = 1; // For Handle Damage

	// Timer
	FTimerHandle AbilityEndTimerHandle;
	FTimerDelegate AbilityEndTimerDelegate;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//
	
	void InitializeTimer(FTimerHandle& InTimerHandle);
	void InitializeComboCount(int32& InCurrentAttackComboCount, int32& InUsedAttackComboCount);

	// Begin Montage
	void SetPlayMontageTask(TMap<int32, UAnimMontage*>& InMontagesMap, int32& InKey);
	UAnimMontage* FindMontageToPlay(TMap<int32, UAnimMontage*>& InMontagesMap, int32& InKey);

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	void SetTimer(FTimerHandle& InTimerHandle, FTimerDelegate& InTimerDelegate);
	void OnAbilityEndTimerFinished();
	// End Montage
	
	void SetWaitMontageEventTask(FGameplayTag& InEventTag);

	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	void ExecuteGameplayCueOnOnwer(FGameplayTag& InGameplayCueTag) const;
	FGameplayEffectSpecHandle MakeAttackDamageSpecHandle(FGameplayEventData& InEventData, TSubclassOf<UGameplayEffect>& InDealDamageEffectClass, FGameplayTag& InSetByCallerAttackTypeTag, int32& InUsedAttackComboCount);
	void HandleDamage(FGameplayEventData& InEventData, FGameplayEffectSpecHandle& InGameplayEffectSpecHandle, FLightAttackTagSet& InTagSet, TSubclassOf<UGameplayEffect>& InGainRageEffectClass);
	void UpdateCurrentAttackComboCount(int32& InCurrentAttackComboCount);
	void ResetCurrentAttackComboCount(int32& InCurrentAttackComboCount);
};
