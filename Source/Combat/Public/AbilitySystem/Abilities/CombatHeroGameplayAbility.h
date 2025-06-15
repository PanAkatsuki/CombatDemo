// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatGameplayAbility.h"
#include "CombatHeroGameplayAbility.generated.h"

class ACombatHeroCharacter;
class ACombatHeroController;
class UHeroFightComponent;
class UHeroUIComponent;

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatHeroGameplayAbility : public UCombatGameplayAbility
{
	GENERATED_BODY()
	
private:
	TWeakObjectPtr<ACombatHeroCharacter> CachedCombatHeroCharacter;
	TWeakObjectPtr<ACombatHeroController> CachedCombatHeroController;

public:
	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	ACombatHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	ACombatHeroController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	UHeroFightComponent* GetHeroFightComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	UHeroUIComponent* GetHeroUIComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectsSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);

	UFUNCTION(BlueprintCallable, Category = "Combat|Ability")
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime);

	
};
