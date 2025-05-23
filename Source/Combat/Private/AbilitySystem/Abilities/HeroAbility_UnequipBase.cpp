// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_UnequipBase.h"

#include "Components/Fight/HeroFightComponent.h"
#include "Items/Weapons/CombatHeroWeapon.h"
#include "AnimInstances/Hero/CombatHeroLinkedAnimLayer.h"
#include "Characters/CombatHeroCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"


ACombatHeroWeapon* UHeroAbility_UnequipBase::GetCurrentEquippedWeapon()
{
	UHeroFightComponent* HeroFightComponent = GetHeroFightComponentFromActorInfo();

	return HeroFightComponent->GetHeroCurrentEquippedWeapon();
}

void UHeroAbility_UnequipBase::HandleUnequippedWeapon()
{
	//UnlinkAnimLayer();

	RemoveMappingContext();

	RemoveWeaponAbilitySet();

	// Should put at last line in UHeroAbility_UnequipBase::HandleUnequippedWeapon()
	GetHeroFightComponentFromActorInfo()->SetCurrentEquippedWeaponAndTag(FGameplayTag(), nullptr);
}

void UHeroAbility_UnequipBase::UnlinkAnimLayer()
{
	UHeroFightComponent* HeroFightComponent = GetHeroFightComponentFromActorInfo();

	ACombatHeroWeapon* HeroWeapon = GetCurrentEquippedWeapon();

	USkeletalMeshComponent* SkeletalMeshComponent = GetOwningComponentFromActorInfo();

	SkeletalMeshComponent->UnlinkAnimClassLayers(HeroWeapon->HeroWeaponData.WeaponAnimLayerToLink);
}

void UHeroAbility_UnequipBase::RemoveMappingContext()
{
	ACombatHeroWeapon* HeroWeapon = GetCurrentEquippedWeapon();

	ULocalPlayer* LocalPlayer = GetHeroCharacterFromActorInfo()->GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(EnhancedInputLocalPlayerSubsystem);

	// AddMappingContext to EnhancedInputLocalPlayerSubsystem
	EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(HeroWeapon->HeroWeaponData.WeaponInputMappingContext);
}

void UHeroAbility_UnequipBase::RemoveWeaponAbilitySet()
{
	TArray<FGameplayAbilitySpecHandle>& AbilitySpecHandles = GetCurrentEquippedWeapon()->GetGrantedAbilitySpecHandles();

	GetCombatAbilitySystemComponentFromActorInfo()->RemoveGrantHeroWeaponAbilities(
		AbilitySpecHandles
	);
}
