// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_EquipBase.generated.h"

class ACombatHeroWeapon;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_EquipBase : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	ACombatHeroWeapon* GetCurrentEquippedWeapon();

protected:
	UFUNCTION(BlueprintCallable)
	void HandleEquippedWeapon();

	void LinkAnimLayer();

	void AddMappingContext();

	void AssignWeaponAbilitySet();

};
