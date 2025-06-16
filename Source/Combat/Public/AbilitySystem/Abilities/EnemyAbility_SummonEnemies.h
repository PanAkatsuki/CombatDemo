// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatEnemyGameplayAbility.h"
#include "EnemyAbility_SummonEnemies.generated.h"


/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyAbility_SummonEnemies : public UCombatEnemyGameplayAbility
{
	GENERATED_BODY()
	
public:
	UEnemyAbility_SummonEnemies();

private:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<ACombatEnemyCharacter> EnemyClassToSpawn;

	UPROPERTY(EditDefaultsOnly)
	int32 NumToSpawn = 1;

	UPROPERTY(EditDefaultsOnly)
	float RandomSpawnRadius = 200.f;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	// Callback function for play montage task
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();
	
	UFUNCTION()
	void OnMontageCancelled();

	// Set Wait Spawn Event Task
	void SetWaitSpawnEventTask(UGameplayAbility* OwningAbility, FGameplayTag InEventTag, TSoftClassPtr<ACombatEnemyCharacter> InSoftEnemyClassToSpawn, int32 InNumToSpawn, const FVector& InSpawnOrigin, float InRandomSpawnRadius);

	// Callback function for Wait Spawn Event Task
	UFUNCTION()
	void OnEventReceived(const TArray<ACombatEnemyCharacter*>& SpawnedEnemies);
};
