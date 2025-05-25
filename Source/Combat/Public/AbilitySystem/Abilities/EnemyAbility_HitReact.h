// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatEnemyGameplayAbility.h"
#include "EnemyAbility_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyAbility_HitReact : public UCombatEnemyGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, UAnimMontage*> MontagesMap;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//bool bHasHitMontagesToPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bFaceToAttacker;

protected:
	UFUNCTION(BlueprintPure)
	bool DoesAbilityHaveMontagesToPlay();

	UFUNCTION(BlueprintPure)
	UAnimMontage* FindMontageToPlay();

	UFUNCTION(BlueprintCallable)
	void MakeEnemyFaceToAttacker(const FGameplayEventData& EventData);
};
