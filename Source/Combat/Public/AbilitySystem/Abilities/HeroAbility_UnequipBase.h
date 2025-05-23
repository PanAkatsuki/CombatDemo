// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_UnequipBase.generated.h"

class ACombatHeroWeapon;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_UnequipBase : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	ACombatHeroWeapon* GetCurrentEquippedWeapon();

protected:
	UFUNCTION(BlueprintCallable)
	void HandleUnequippedWeapon();

	void UnlinkAnimLayer();

	void RemoveMappingContext();

	void RemoveWeaponAbilitySet();
	
};
