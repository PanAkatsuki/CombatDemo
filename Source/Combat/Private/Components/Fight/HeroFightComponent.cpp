// Zhang All Rights Reserved.


#include "Components/Fight/HeroFightComponent.h"

#include "Items/Weapons/CombatHeroWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CombatGameplayTags.h"
#include "CombatFunctionLibrary.h"

#include "CombatDebugHelper.h"

void UHeroFightComponent::OnHitTargetActor(AActor* HitActor)
{
    // For boss with multiply collision box
    if (AttackOverlappedActors.Contains(HitActor))
    {
        return;
    }

    Super::OnHitTargetActor(HitActor);

    FGameplayEventData EventData;
    EventData.Instigator = GetOwner();
    EventData.Target = HitActor;

    if (UCombatFunctionLibrary::NativeDoesActorHaveTag(GetOwner(), CombatGameplayTags::Player_Status_CounterAttacking))
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), CombatGameplayTags::Player_Event_CounterAttack, EventData); 
    }
    else if (UCombatFunctionLibrary::NativeDoesActorHaveTag(GetOwner(), CombatGameplayTags::Player_Status_SpecialAbilityAttacking))
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), CombatGameplayTags::Player_Event_SpecialAbilityAttack, EventData);
    }
    else
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), CombatGameplayTags::Shared_Event_MeleeHit, EventData);
    }

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), CombatGameplayTags::Player_Event_HitPause, FGameplayEventData());
}

void UHeroFightComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
    Super::OnWeaponPulledFromTargetActor(InteractedActor);

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), CombatGameplayTags::Player_Event_HitPause, FGameplayEventData());
}

ACombatHeroWeapon* UHeroFightComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<ACombatHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

ACombatHeroWeapon* UHeroFightComponent::GetHeroCurrentEquippedWeapon() const
{
    return Cast<ACombatHeroWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UHeroFightComponent::GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const
{
    return GetHeroCurrentEquippedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}
