// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatEnemyGameplayAbility.h"
#include "EnemyAbility_Projectile.generated.h"


class ACombatProjectileBase;


USTRUCT(BlueprintType)
struct FEnemyProjectileAttackTagSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WaitMontageEventTag;
};

/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyAbility_Projectile : public UCombatEnemyGameplayAbility
{
	GENERATED_BODY()
	
public:
	UEnemyAbility_Projectile();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TSubclassOf<ACombatProjectileBase> ProjectileToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TSubclassOf<UGameplayEffect> DealDamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FScalableFloat ScalableFloat;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FEnemyProjectileAttackTagSet TagSet;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	// Callback Function For Play Montage Task
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	// Callback Function For Wait Gameplay Tag Task
	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);
};
