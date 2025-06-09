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

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, UAnimMontage*> MontagesMap;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACombatProjectileBase> ProjectileToSpawn;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DealDamageEffect;

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat ScalableFloat;

	UPROPERTY(EditDefaultsOnly)
	FEnemyProjectileAttackTagSet TagSet;

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

	// Set Wait Montage Event Task
	void SetWaitMontageEventTask(FGameplayTag& InEventTag);

	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);
};
