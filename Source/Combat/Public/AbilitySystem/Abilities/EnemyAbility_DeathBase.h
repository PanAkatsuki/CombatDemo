// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatEnemyGameplayAbility.h"
#include "EnemyAbility_DeathBase.generated.h"

class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyAbility_DeathBase : public UCombatEnemyGameplayAbility
{
	GENERATED_BODY()
	
public:
	UEnemyAbility_DeathBase();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FGameplayTag DeathSoundGameplayCueTag;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TSoftObjectPtr<UNiagaraSystem> DissolveNiagaraSystem;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

};
