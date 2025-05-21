// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Fight/PawnFightComponent.h"
#include "EnemyFightComponent.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UEnemyFightComponent : public UPawnFightComponent
{
	GENERATED_BODY()
	
public:
	// Function Bind To Delegater
	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;

protected:
	virtual void ToggleBodyAttackCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType) override;

};
