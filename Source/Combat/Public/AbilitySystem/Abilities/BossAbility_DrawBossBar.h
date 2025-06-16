// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatEnemyGameplayAbility.h"
#include "BossAbility_DrawBossBar.generated.h"


class UCombatWidgetBase;

/**
 * 
 */
UCLASS()
class COMBAT_API UBossAbility_DrawBossBar : public UCombatEnemyGameplayAbility
{
	GENERATED_BODY()
	
public:
	UBossAbility_DrawBossBar();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TSubclassOf<UCombatWidgetBase> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FText BossName;

	UCombatWidgetBase* HealthBarWidget;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
