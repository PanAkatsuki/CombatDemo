// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatBaseAnimInstance.generated.h"

struct FGameplayTag;

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag TogToCheck) const;
};
