// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_TimeCounter.generated.h"


USTRUCT(BlueprintType)
struct FTimeCounterTagSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponHitSoundGameplayCueTag;
};

USTRUCT(BlueprintType)
struct FTimeCounterEffectSet
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
class COMBAT_API UHeroAbility_TimeCounter : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()

public:
	UHeroAbility_TimeCounter();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FTimeCounterTagSet TagSet;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FTimeCounterEffectSet EffectSet;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	float TimeCounterDistance = 200.f;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UAnimMontage* FindMontageToPlayWithKey(TMap<int32, UAnimMontage*>& InAnimMontagesMap, int32 InKey);
	void FindNearestPointAroundTargetAndSetMotionWarping(AActor* SelfActor, AActor* TargetActor, float DesiredDistance);

	UFUNCTION()
	void OnRollCompleted();

	// Callback function for play montage task
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	// Wait Event Task
	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);
};
