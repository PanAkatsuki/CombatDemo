// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_HeavyAttackBase.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_HeavyAttackBase : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<int32, UAnimMontage*> AttackMontagesMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentAttackComboCount = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 UsedAttackComboCount = 1; // For Handle Damage

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponHitSoundGameplayCueTag;

protected:
	UFUNCTION(BlueprintPure)
	UAnimMontage* FindMontageToPlay();

	UFUNCTION(BlueprintCallable)
	void InitializeComboCount();

	UFUNCTION(BlueprintCallable)
	void UpdateCurrentAttackComboCount();
};
