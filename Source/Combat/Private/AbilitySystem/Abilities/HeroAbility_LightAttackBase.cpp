// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_LightAttackBase.h"

#include "Characters/CombatHeroCharacter.h"
#include "CombatFunctionLibrary.h"
#include "CombatGameplayTags.h"
#include "TimerManager.h"
#include "AbilitySystemComponent.h"
#include "Components/Fight/HeroFightComponent.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

#include "CombatDebugHelper.h"


UHeroAbility_LightAttackBase::UHeroAbility_LightAttackBase()
{
	
}

UAnimMontage* UHeroAbility_LightAttackBase::FindMontageToPlay()
{
	//Debug::Print(TEXT("CurrentAttackComboCount"), CurrentAttackComboCount);

	UAnimMontage* const* MontagePtr = AttackMontagesMap.Find(CurrentAttackComboCount);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UHeroAbility_LightAttackBase::InitializeComboCount()
{
	AActor* Hero = Cast<AActor>(GetHeroCharacterFromActorInfo());

	check(Hero);

	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(Hero, CombatGameplayTags::Player_Status_JumpToFinisher))
	{
		CurrentAttackComboCount = AttackMontagesMap.Num();
	}

	UsedAttackComboCount = CurrentAttackComboCount;
}

void UHeroAbility_LightAttackBase::ExecuteLightAttackGameplayCue(FGameplayTag& InGameplayCueTag)
{
	ACombatHeroCharacter* Hero = GetHeroCharacterFromActorInfo();

	check(Hero);

	if (UCombatAbilitySystemComponent* CASC = Hero->GetCombatAbilitySystemComponent())
	{
		CASC->ExecuteGameplayCue(InGameplayCueTag, FGameplayEffectContextHandle());
	}
}

void UHeroAbility_LightAttackBase::UpdateCurrentAttackComboCount()
{
	AActor* Hero = Cast<AActor>(GetHeroCharacterFromActorInfo());

	check(Hero);

	if (CurrentAttackComboCount >= AttackMontagesMap.Num())
	{
		ResetCurrentAttackComboCount();
		UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(Hero, CombatGameplayTags::Player_Status_JumpToFinisher);
	}
	else
	{
		// Should Add Tag Jump to finisher
		if (CurrentAttackComboCount == AttackMontagesMap.Num() - 1)
		{
			UCombatFunctionLibrary::AddGameplayTagToActorIfNone(Hero, CombatGameplayTags::Player_Status_JumpToFinisher);
		}
		else
		{
			UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(Hero, CombatGameplayTags::Player_Status_JumpToFinisher);
		}
		
		CurrentAttackComboCount++;
	}
}

void UHeroAbility_LightAttackBase::ResetCurrentAttackComboCount()
{
	CurrentAttackComboCount = 1;
}