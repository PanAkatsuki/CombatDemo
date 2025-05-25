// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_HeavyAttackBase.h"

#include "CombatFunctionLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"


UAnimMontage* UHeroAbility_HeavyAttackBase::FindMontageToPlay()
{
	//Debug::Print(TEXT("FindMontageToPlay()CurrentAttackComboCount"), CurrentAttackComboCount);

	UAnimMontage* const* MontagePtr = AttackMontagesMap.Find(CurrentAttackComboCount);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UHeroAbility_HeavyAttackBase::InitializeComboCount()
{
	//Debug::Print(TEXT("InitializeComboCount()CurrentAttackComboCount"), CurrentAttackComboCount);

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
		ResetCurrentAttackComboCount();
		//Debug::Print(TEXT("I1"));
	}
	else
	{
		++CurrentAttackComboCount;
		//Debug::Print(TEXT("I2"));
	}

	//Debug::Print(TEXT("UpdateCurrentAttackComboCount()CurrentAttackComboCount"), CurrentAttackComboCount);
}

void UHeroAbility_HeavyAttackBase::ResetCurrentAttackComboCount()
{
	CurrentAttackComboCount = 1;
}