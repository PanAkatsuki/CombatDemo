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
	FGameplayTag SoundGameplayCueTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UNiagaraSystem> DissolveNiagaraSystem;

protected:
	UFUNCTION(BlueprintPure)
	UAnimMontage* FindMontageToPlay();
};
