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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, UAnimMontage*> MontagesMap;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//bool bHasHitMontagesToPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bFaceToAttacker = true;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> UnderAttackEffect;

	FLatentActionInfo HitReactLatentInfo;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	UFUNCTION(BlueprintCallable)
	void MakeEnemyFaceToAttacker(const FGameplayEventData& EventData);

	UFUNCTION(BlueprintPure)
	bool DoesAbilityHaveMontagesToPlay();

	void SetPlayMontageTask(TMap<int32, UAnimMontage*>& InMontagesMap);
	UFUNCTION(BlueprintPure)
	UAnimMontage* FindMontageToPlay(TMap<int32, UAnimMontage*>& InMontagesMap);

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnHitReactDelayFinished();
};
