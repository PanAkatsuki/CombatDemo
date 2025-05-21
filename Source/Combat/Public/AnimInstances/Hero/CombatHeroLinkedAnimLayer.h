// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CombatBaseAnimInstance.h"
#include "CombatHeroLinkedAnimLayer.generated.h"

class UCombatHeroAnimInstance;

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatHeroLinkedAnimLayer : public UCombatBaseAnimInstance
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UCombatHeroAnimInstance> CachedHeroAnimInstance;
	
public:
	UFUNCTION(BlueprintPure, meta = (NotBlueprintThreadSafe))
	UCombatHeroAnimInstance* GetCombatHeroAnimInstance();

};
