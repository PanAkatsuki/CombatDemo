// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_HeavyAttackBase.h"

#include "CombatFunctionLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "CombatGameplayTags.h"


UAnimMontage* UHeroAbility_HeavyAttackBase::FindMontageToPlay()
{
	UAnimMontage* const* MontagePtr = AttackMontagesMap.Find(CurrentAttackComboCount);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UHeroAbility_HeavyAttackBase::InitializeComboCount()
{
	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_JumpToFinisher))
	{
		CurrentAttackComboCount = AttackMontagesMap.Num();
	}

	UsedAttackComboCount = CurrentAttackComboCount;
}

void UHeroAbility_HeavyAttackBase::UpdateCurrentAttackComboCount()
{
	if (CurrentAttackComboCount == AttackMontagesMap.Num())
	{
		CurrentAttackComboCount = 1;
	}
	else
	{
		++CurrentAttackComboCount;
	}
}