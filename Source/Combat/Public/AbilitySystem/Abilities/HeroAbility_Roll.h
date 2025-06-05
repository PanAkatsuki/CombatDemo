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
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat RollDistanceScalableFloat;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	FLatentActionInfo LatentInfo;
	
protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	UFUNCTION()
	void OnDelayFinished();

	void ComputeRollDiractionAndDistance();

	void SetPlayMontageTask(UAnimMontage* InMontageToPlay);

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();
};
