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
	ACombatHeroCharacter* GetHeroCharacterFromActorInfo();
	ACombatHeroController* GetHeroControllerFromActorInfo();
	UHeroFightComponent* GetHeroFightComponentFromActorInfo();
	UHeroUIComponent* GetHeroUIComponentFromActorInfo();
	FGameplayEffectSpecHandle MakeHeroDamageEffectsSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime);
};
