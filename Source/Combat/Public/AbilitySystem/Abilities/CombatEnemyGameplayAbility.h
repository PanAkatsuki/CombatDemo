// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatGameplayAbility.h"
#include "CombatEnemyGameplayAbility.generated.h"

class ACombatEnemyCharacter;
class UEnemyFightComponent;

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatEnemyGameplayAbility : public UCombatGameplayAbility
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<ACombatEnemyCharacter> CachedEnemyCharacter;

public:
	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	ACombatEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	UEnemyFightComponent* GetEnemyFightComponentFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat);
};
