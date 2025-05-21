// Zhang All Rights Reserved.


#include "AnimInstances/CombatBaseAnimInstance.h"

#include "GameplayTagContainer.h"
#include "CombatFunctionLibrary.h"


bool UCombatBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TogToCheck) const
{
	if (APawn* PawnOwner = TryGetPawnOwner())
	{
		return UCombatFunctionLibrary::NativeDoesActorHaveTag(PawnOwner, TogToCheck);

	}

	return false;
}