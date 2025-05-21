// Zhang All Rights Reserved.


#include "CombatTypes/CombatStructTypes.h"

#include "InputMappingContext.h"


bool FCombatHeroAbilitySets::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
