// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_CounterAttack.h"

#include "Characters/CombatHeroCharacter.h"
#include "Components/Fight/HeroFightComponent.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CombatFunctionLibrary.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"


UHeroAbility_CounterAttack::UHeroAbility_CounterAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_CounterAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask(this, FName("CounterAttackMontageTask"), FindMontageToPlayByRandom(AnimMontagesMap));
	SetWaitMontageEventTask(this, CombatGameplayTags::Player_Event_CounterAttack);
}

void UHeroAbility_CounterAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);
	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_CanCounterAttack);
	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_JumpToFinisher);
}

void UHeroAbility_CounterAttack::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_CounterAttack::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_CounterAttack::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_CounterAttack::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_CounterAttack::OnEventReceived(FGameplayEventData InEventData)
{
	// Play Gameplay Cue
	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.NormalizedMagnitude = 1.f;
	GameplayCueParameters.Location = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(TagSet.WeaponHitSoundGameplayCueTag, GameplayCueParameters);

	// Make Damage Spec Handle
	float BaseWeaponDamage = GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel());
	const int32 UsedComboCount = 1;
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeHeroDamageEffectsSpecHandle(
		DealDamageEffectClass,
		BaseWeaponDamage,
		CombatGameplayTags::Player_SetByCaller_AttackType_CounterAttack,
		UsedComboCount
	);

	AActor* TargetActor = const_cast<AActor*>(InEventData.Target.Get());
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, GameplayEffectSpecHandle);

	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, CombatGameplayTags::Shared_Event_HitReact, InEventData);
	}
}
