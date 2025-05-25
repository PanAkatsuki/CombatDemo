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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FLatentActionInfo LatentInfo;

protected:
	UFUNCTION(BlueprintCallable)
	void ActivateHitPauseAbility();

	
};

