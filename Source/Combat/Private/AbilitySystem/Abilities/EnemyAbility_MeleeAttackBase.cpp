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
	SetPlayMontageTask(MontageToPlay);
	SetWaitMontageEventTask(TagSet.WaitMontageEventTag);
}

void UEnemyAbility_MeleeAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

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

void UEnemyAbility_MeleeAttackBase::SetPlayMontageTask(UAnimMontage* InMontageToPlay)
{
	check(InMontageToPlay);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayEnemyMeleeAttackMontageTask"),
		InMontageToPlay,
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

void UEnemyAbility_MeleeAttackBase::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false, AttackType);
}

void UEnemyAbility_MeleeAttackBase::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false, AttackType);
}

void UEnemyAbility_MeleeAttackBase::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false, AttackType);
}

void UEnemyAbility_MeleeAttackBase::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false, AttackType);
}

void UEnemyAbility_MeleeAttackBase::SetWaitMontageEventTask(FGameplayTag& InWaitMontageEventTag)
{
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		InWaitMontageEventTag,
		nullptr,
		false,
		true
	);

	WaitEventTask->EventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived);

	WaitEventTask->ReadyForActivation();
}

void UEnemyAbility_MeleeAttackBase::OnEventReceived(FGameplayEventData InEventData)
{
	ExecuteGameplayCueOnOnwer(TagSet.WeaponHitSoundGameplayCueTag);
	FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeEnemyDamageEffectSpecHandle(
		Effect,
		InDamageScalableFloat
	);
	HandleDamage(InEventData, DamageEffectSpecHandle, TagSet);
}

void UEnemyAbility_MeleeAttackBase::ExecuteGameplayCueOnOnwer(FGameplayTag& InGameplayCueTag) const
{
	// Should make sure InGameplayCueTag is a GameplayCueTag

	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.NormalizedMagnitude = 1.f;
	GameplayCueParameters.Location = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(InGameplayCueTag, GameplayCueParameters);
}

void UEnemyAbility_MeleeAttackBase::HandleDamage(FGameplayEventData& InEventData, FGameplayEffectSpecHandle& InGameplayEffectSpecHandle, FEnemyMeleeAttackTagSet& InTagSet)
{
	AActor* TargetActor = const_cast<AActor*>(InEventData.Target.Get());
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InGameplayEffectSpecHandle);

	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, InTagSet.TargetHitReactEventTag, InEventData);
	}
	else
	{
		Debug::Print(TEXT("In UEnemyAbility_MeleeAttackBase::HandleDamage, ActiveGameplayEffectHandle was not successfully applied!"));
	}
}
