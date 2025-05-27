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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, UAnimMontage*> MontagesMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DeathSoundGameplayCueTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UNiagaraSystem> DissolveNiagaraSystem;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

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

	void ExecuteGameplayCueOnOnwer(FGameplayTag& InGameplayCueTag) const;

};
