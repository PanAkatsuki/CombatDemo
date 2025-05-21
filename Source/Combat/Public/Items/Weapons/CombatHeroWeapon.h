// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/CombatWeaponBase.h"

#include "CombatTypes/CombatStructTypes.h"
#include "GameplayAbilitySpecHandle.h"

#include "CombatHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatHeroWeapon : public ACombatWeaponBase
{
	GENERATED_BODY()
	
private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FCombatHeroWeaponData HeroWeaponData;

public:
	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InAbilitySpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
}; 
