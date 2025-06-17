// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/EnemyAbility_MeleeAttackBase.h"

#include "CombatFunctionLibrary.h"
#include "CombatGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/Fight/PawnFightComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "CombatDebugHelper.h"


UEnemyAbility_MeleeAttackBase::UEnemyAbility_MeleeAttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEnemyAbility_MeleeAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CheckIfShouldPlayUnblockableAttackWarning();
	SetPlayMontageTask(this, FName("PlayEnemyMeleeMontageTask"), FindMontageToPlayByRandom(AnimMontagesMap));
	SetWaitMontageEventTask(this, CombatGameplayTags::Shared_Event_MeleeHit);
}

void UEnemyAbility_MeleeAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false, AttackType);
}

void UEnemyAbility_MeleeAttackBase::CheckIfShouldPlayUnblockableAttackWarning()
{
	if (AActor* EnemyActor = CurrentActorInfo->AvatarActor.Get())
	{
		if (UCombatFunctionLibrary::NativeDoesActorHaveTag(EnemyActor, CombatGameplayTags::Enemy_Status_Unblockable))
		{
			FGameplayCueParameters GameplayCueParameters;
			GameplayCueParameters.RawMagnitude = UnblockableAttackWarningSpawnOffset;
			K2_ExecuteGameplayCueWithParams(TagSet.UnBlockableWarningGameplayCueTag, GameplayCueParameters);
		}
	}
	else
	{
		Debug::Print(TEXT("In CheckIfShouldPlayUnblockableAttackWarning, EnemyActor is null!"));
	}
}

void UEnemyAbility_MeleeAttackBase::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEnemyAbility_MeleeAttackBase::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEnemyAbility_MeleeAttackBase::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UEnemyAbility_MeleeAttackBase::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UEnemyAbility_MeleeAttackBase::OnEventReceived(FGameplayEventData InEventData)
{
	// Check Invincible
	AActor* TargetActor = const_cast<AActor*>(InEventData.Target.Get());
	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(TargetActor, CombatGameplayTags::Shared_Status_Invincible))
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, CombatGameplayTags::Player_Event_PerfectRoll, InEventData);
		return;
	}

	// Play Gameplay Cue
	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.NormalizedMagnitude = 1.f;
	GameplayCueParameters.Location = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(TagSet.WeaponHitSoundGameplayCueTag, GameplayCueParameters);

	// Make Damage Effect Spec Handle
	FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeEnemyDamageEffectSpecHandle(
		Effect,
		InDamageScalableFloat
	);

	// Apply Damage Effect Spec To Target
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, DamageEffectSpecHandle);

	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, TagSet.TargetHitReactEventTag, InEventData);
	}
	else
	{
		// TargeActor has tag invisible
	}
}
