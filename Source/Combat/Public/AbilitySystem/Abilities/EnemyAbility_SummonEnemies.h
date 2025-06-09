// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatEnemyGameplayAbility.h"
#include "EnemyAbility_SummonEnemies.generated.h"


//USTRUCT(BlueprintType)
//struct FEnemySummonTagSet
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(EditDefaultsOnly)
//	FGameplayTag WaitMontageEventTag;
//};

/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyAbility_SummonEnemies : public UCombatEnemyGameplayAbility
{
	GENERATED_BODY()
	
public:
	UEnemyAbility_SummonEnemies();

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, UAnimMontage*> MontagesMap;

	/*UPROPERTY(EditDefaultsOnly)
	FEnemySummonTagSet TagSet;*/

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<ACombatEnemyCharacter> EnemyClassToSpawn;

	UPROPERTY(EditDefaultsOnly)
	int32 NumToSpawn = 1;

	UPROPERTY(EditDefaultsOnly)
	float RandomSpawnRadius = 200.f;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	// Montage task
	void SetPlayMontageTask();
	UAnimMontage* FindMontageToPlay();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	//Wait event task
	void SetWaitSpawnEventTask();

	UFUNCTION()
	void OnEventReceived(const TArray<ACombatEnemyCharacter*>& SpawnedEnemies);
};
