// Zhang All Rights Reserved.


#include "Components/Fight/PawnFightComponent.h"

#include "Items/Weapons/CombatWeaponBase.h"
#include "Components/BoxComponent.h"

#include "CombatDebugHelper.h"

void UPawnFightComponent::OnHitTargetActor(AActor* HitActor)
{
	if (AttackOverlappedActors.Contains(HitActor))
	{
		return;
	}

	AttackOverlappedActors.AddUnique(HitActor);
}

void UPawnFightComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}

void UPawnFightComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, ACombatWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("%s has been registered!"), *InWeaponTagToRegister.ToString());
	checkf(InWeaponToRegister, TEXT("InWeaponToRegister is a nullptr"));

	// Add Weapon to Carried Weapon Map
	// This Demo contains only ONE Weapon!
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	// Bind To Weapon Class's Delegater
	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
		CurrentEquippedWeapon = InWeaponToRegister;
	}
}

void UPawnFightComponent::SetCurrentEquippedWeaponAndTag(FGameplayTag InWeaponTag, ACombatWeaponBase* InWeapon)
{
	CurrentEquippedWeaponTag = InWeaponTag;
	CurrentEquippedWeapon = InWeapon;
}

void UPawnFightComponent::ClearCurrentEquippedWeaponAndTag()
{
	CurrentEquippedWeaponTag = FGameplayTag();
	CurrentEquippedWeapon = nullptr;
}

ACombatWeaponBase* UPawnFightComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		if (ACombatWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

ACombatWeaponBase* UPawnFightComponent::GetCharacterCurrentEquippedWeapon() const
{
	return CurrentEquippedWeapon;
}

void UPawnFightComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
	}
	else
	{
		ToggleBodyAttackCollision(bShouldEnable, ToggleDamageType);
	}
}

void UPawnFightComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
{
	ACombatWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();

	//checkf(WeaponToToggle, TEXT("WeaponToToggle is not valid."));

	if (!WeaponToToggle)
	{
		Debug::Print(TEXT("PawnFightComponent.cpp line 34: WeaponToToggle is not valid."));
		return;
	}

	if (bShouldEnable)
	{
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Empty overlapped actors should be calld when set weapon collision to no collision
		AttackOverlappedActors.Empty();
	}
}

void UPawnFightComponent::ToggleBodyAttackCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{

}