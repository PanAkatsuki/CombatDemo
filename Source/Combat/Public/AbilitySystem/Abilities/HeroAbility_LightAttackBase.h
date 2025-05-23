// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_LightAttackBase.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_LightAttackBase : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_LightAttackBase();

protected:
	

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
	void ExecuteLightAttackGameplayCue(FGameplayTag& InGameplayCueTag);

	UFUNCTION(BlueprintCallable)
	void UpdateCurrentAttackComboCount();

	// Navitve Function
	void ResetCurrentAttackComboCount();
};
