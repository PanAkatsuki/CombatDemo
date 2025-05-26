// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/EnemyAbility_DeathBase.h"

#include "Characters/CombatEnemyCharacter.h"


UEnemyAbility_DeathBase::UEnemyAbility_DeathBase()
{
	//Cast<ACombatEnemyCharacter>(GetAvatarActorFromActorInfo())->OnAsyncLoadFinishedDelegate.BindUObject()
}

UAnimMontage* UEnemyAbility_DeathBase::FindMontageToPlay()
{
	int32 RandomInt = FMath::RandRange(1, MontagesMap.Num());

	UAnimMontage* const* MontagePtr = MontagesMap.Find(RandomInt);

	return MontagePtr ? *MontagePtr : nullptr;
}
