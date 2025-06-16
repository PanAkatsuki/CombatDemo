// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatEnemyGameplayAbility.h"
#include "EnemyAbility_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyAbility_HitReact : public UCombatEnemyGameplayAbility
{
	GENERATED_BODY()

public:
	UEnemyAbility_HitReact();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	bool bFaceToAttacker = true;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TSubclassOf<UGameplayEffect> UnderAttackEffect;

	FLatentActionInfo HitReactLatentInfo;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void MakeEnemyFaceToAttacker(const FGameplayEventData& EventData);

	bool DoesAbilityHaveMontagesToPlay();

	// Callback Function For Play Montage Task
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	// Latent
	UFUNCTION()
	void OnHitReactDelayFinished();
};
