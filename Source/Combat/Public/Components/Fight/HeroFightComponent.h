// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Fight/PawnFightComponent.h"
#include "HeroFightComponent.generated.h"

class ACombatHeroWeapon;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroFightComponent : public UPawnFightComponent
{
	GENERATED_BODY()

public:
	// Function Bind To Delegater
	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;

private:
	TWeakObjectPtr<ACombatHeroWeapon> CachedHeroCarriedWeapon;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Combat|Fight")
	ACombatHeroWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Fight")
	ACombatHeroWeapon* GetHeroCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Fight")
	float GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const;
	
};
