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
	
private:
	UPROPERTY(EditAnywhere, Category = "Combat|Ability")
	float OriginalWorldDilation = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combat|Ability")
	float HitPauseTimeDilation = 0.15f;

	UPROPERTY(EditAnywhere, Category = "Combat|Ability")
	float DelayDuration = 0.02f;

	UPROPERTY(EditAnywhere, Category = "Combat|Ability")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	FLatentActionInfo HitPauseLatentActionInfo;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void ActivateHitPauseAbility();

	// Callback funtion for lantend
	UFUNCTION()
	void OnHitPauseDelayFinished();
};

