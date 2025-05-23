// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_EquipBase.h"

#include "Components/Fight/HeroFightComponent.h"
#include "CombatGameplayTags.h"
#include "Items/Weapons/CombatHeroWeapon.h"
#include "AnimInstances/Hero/CombatHeroLinkedAnimLayer.h"
#include "Characters/CombatHeroCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

#include "CombatDebugHelper.h"


ACombatHeroWeapon* UHeroAbility_EquipBase::GetCurrentEquippedWeapon()
{
	UHeroFightComponent* HeroFightComponent = GetHeroFightComponentFromActorInfo();

	return HeroFightComponent->GetHeroCurrentEquippedWeapon();
}

void UHeroAbility_EquipBase::HandleEquippedWeapon()
{
	ACombatHeroWeapon* WeaponAxe = GetHeroFightComponentFromActorInfo()->GetHeroCarriedWeaponByTag(CombatGameplayTags::Player_Weapon_Axe);

	GetHeroFightComponentFromActorInfo()->SetCurrentEquippedWeaponAndTag(CombatGameplayTags::Player_Weapon_Axe, WeaponAxe);

	LinkAnimLayer();

	AddMappingContext();

	AssignWeaponAbilitySet();
}

void UHeroAbility_EquipBase::LinkAnimLayer()
{
	UHeroFightComponent* HeroFightComponent = GetHeroFightComponentFromActorInfo();

	ACombatHeroWeapon* HeroWeapon = GetCurrentEquippedWeapon();

	USkeletalMeshComponent* SkeletalMeshComponent = GetOwningComponentFromActorInfo();

	SkeletalMeshComponent->LinkAnimClassLayers(HeroWeapon->HeroWeaponData.WeaponAnimLayerToLink);
}

void UHeroAbility_EquipBase::AddMappingContext()
{
	ACombatHeroWeapon* HeroWeapon = GetCurrentEquippedWeapon();

	ULocalPlayer* LocalPlayer = GetHeroCharacterFromActorInfo()->GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(EnhancedInputLocalPlayerSubsystem);

	// AddMappingContext to EnhancedInputLocalPlayerSubsystem
	EnhancedInputLocalPlayerSubsystem->AddMappingContext(HeroWeapon->HeroWeaponData.WeaponInputMappingContext, 1);
}

void UHeroAbility_EquipBase::AssignWeaponAbilitySet()
{
	UCombatAbilitySystemComponent* CombatAbilitySystemComponent = GetCombatAbilitySystemComponentFromActorInfo();

	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandle;

	CombatAbilitySystemComponent->GrantHeroWeaponAbilities(
		GetCurrentEquippedWeapon()->HeroWeaponData.DefaultWeaponAbilities,
		GetCurrentEquippedWeapon()->HeroWeaponData.SpecialWeaponAbilities,
		GetAbilityLevel(),
		AbilitySpecHandle
	);

	GetCurrentEquippedWeapon()->AssignGrantedAbilitySpecHandles(AbilitySpecHandle);
}
