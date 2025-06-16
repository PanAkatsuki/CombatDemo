// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_SpecialLight.h"

#include "CombatGameplayTags.h"
#include "Components/Fight/HeroFightComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "Components/UI/HeroUIComponent.h"

#include "CombatDebugHelper.h"


UHeroAbility_SpecialLight::UHeroAbility_SpecialLight()
{
}

void UHeroAbility_SpecialLight::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask(this, FName("SpecialLightMontageTask"), FindMontageToPlayByRandom(AnimMontagesMap));
	SetWaitMontageEventTask(this, CombatGameplayTags::Player_Event_SpecialAbilityAttack);

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	GetHeroCharacterFromActorInfo()->GetHeroUIComponent()->OnAbilityCooldownBegin.Broadcast(
		CombatGameplayTags::InputTag_SpecialWeaponAbility_Light,
		GetCooldownTimeRemaining(),
		GetCooldownTimeRemaining()
	);
}

void UHeroAbility_SpecialLight::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UHeroAbility_SpecialLight::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_SpecialLight::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_SpecialLight::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_SpecialLight::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_SpecialLight::OnEventReceived(FGameplayEventData InEventData)
{
	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.NormalizedMagnitude = 1.f;
	GameplayCueParameters.Location = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(TagSet.WeaponHitSoundGameplayCueTag, GameplayCueParameters);

	float BaseWeaponDamage = GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel());
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeHeroDamageEffectsSpecHandle(
		EffectSet.DealDamageEffectClass,
		BaseWeaponDamage,
		CombatGameplayTags::Player_SetByCaller_AttackType_SpecialWeaponAbility,
		1
	);

	AActor* TargetActor = const_cast<AActor*>(InEventData.Target.Get());
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, GameplayEffectSpecHandle);

	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, CombatGameplayTags::Shared_Event_HitReact, InEventData);

		ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSet.GainRageEffectClass.GetDefaultObject(), GetAbilityLevel());
	}
}