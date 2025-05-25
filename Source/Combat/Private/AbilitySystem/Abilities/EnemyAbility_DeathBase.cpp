// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/EnemyAbility_DeathBase.h"


UAnimMontage* UEnemyAbility_DeathBase::FindMontageToPlay()
{
	int32 RandomInt = FMath::RandRange(1, MontagesMap.Num());

	UAnimMontage* const* MontagePtr = MontagesMap.Find(RandomInt);

	return MontagePtr ? *MontagePtr : nullptr;
}
