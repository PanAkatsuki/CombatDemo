// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_Roll.generated.h"



/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_Roll : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_Roll();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FScalableFloat RollDistanceScalableFloat;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	FLatentActionInfo RollLatentInfo;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

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
};
