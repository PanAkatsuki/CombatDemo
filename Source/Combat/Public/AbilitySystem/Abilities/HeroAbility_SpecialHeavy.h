// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_SpecialHeavy.generated.h"


USTRUCT(BlueprintType)
struct FHeavySpecialAttackTagSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponHitSoundGameplayCueTag;
};

USTRUCT(BlueprintType)
struct FHeavySpecialAttackEffectSet
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
class COMBAT_API UHeroAbility_SpecialHeavy : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()

public:
	UHeroAbility_SpecialHeavy();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FHeavySpecialAttackTagSet TagSet;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FHeavySpecialAttackEffectSet EffectSet;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

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
};
