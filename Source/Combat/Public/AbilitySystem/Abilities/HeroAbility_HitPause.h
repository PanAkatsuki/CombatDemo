// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_HitPause.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_HitPause : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_HitPause();

protected:
	UFUNCTION()
	void OnHitPauseDelayFinished();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OriginalWorldDilation = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HitPauseTimeDilation = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DelayDuration = 0.02f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	FLatentActionInfo LatentInfo;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	UFUNCTION(BlueprintCallable)
	void ActivateHitPauseAbility();

	
};

