// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_CounterAttack.generated.h"


USTRUCT(BlueprintType)
struct FCounterAttackTagSet
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

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_CounterAttack : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_CounterAttack();

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, UAnimMontage*> MontageSet;

	UPROPERTY(EditDefaultsOnly)
	FCounterAttackTagSet TagSet;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	// Set Montage Task
	void SetPlayMontageTask(TMap<int32, UAnimMontage*>& InMontagesMap);
	UAnimMontage* FindMontageToPlay(TMap<int32, UAnimMontage*>& InMontagesMap);

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	void SetWaitMontageEventTask(FGameplayTag& InEventTag);

	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	void ExecuteGameplayCueOnOnwer(FGameplayTag& InGameplayCueTag) const;
	FGameplayEffectSpecHandle MakeAttackDamageSpecHandle(TSubclassOf<UGameplayEffect>& InDealDamageEffectClass, FGameplayTag& InSetByCallerAttackTypeTag, int32& InUsedAttackComboCount);
	void HandleDamage(FGameplayEventData& InEventData, FGameplayEffectSpecHandle& InGameplayEffectSpecHandle, FCounterAttackTagSet& InTagSet);
};
