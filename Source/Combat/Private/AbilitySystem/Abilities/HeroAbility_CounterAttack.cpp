// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_CounterAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/CombatHeroCharacter.h"
#include "Components/Fight/HeroFightComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "CombatDebugHelper.h"


UHeroAbility_CounterAttack::UHeroAbility_CounterAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_CounterAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask(MontageSet);
	SetWaitMontageEventTask(TagSet.WaitMontageEventTag);
}

void UHeroAbility_CounterAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UHeroAbility_CounterAttack::SetPlayMontageTask(TMap<int32, UAnimMontage*>& InMontagesMap)
{
	check(InMontagesMap.Num());

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayCounterAttackMontageTask"),
		FindMontageToPlay(InMontagesMap),
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

UAnimMontage* UHeroAbility_CounterAttack::FindMontageToPlay(TMap<int32, UAnimMontage*>& InMontagesMap)
{
	int32 RandomInt = FMath::RandRange(1, InMontagesMap.Num());

	UAnimMontage* const* MontagePtr = InMontagesMap.Find(RandomInt);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UHeroAbility_CounterAttack::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);
}

void UHeroAbility_CounterAttack::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);
}

void UHeroAbility_CounterAttack::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);
}

void UHeroAbility_CounterAttack::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);
}

void UHeroAbility_CounterAttack::SetWaitMontageEventTask(FGameplayTag& InEventTag)
{
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		InEventTag,
		nullptr,
		false,
		true
	);

	WaitEventTask->EventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived);

	WaitEventTask->ReadyForActivation();
}

void UHeroAbility_CounterAttack::OnEventReceived(FGameplayEventData InEventData)
{
	ExecuteGameplayCueOnOnwer(TagSet.WeaponHitSoundGameplayCueTag);

	int32 ComboCount = 1;
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeAttackDamageSpecHandle(
		GameplayEffect,
		TagSet.SetByCallerAttackTypeTag,
		ComboCount
	);

	HandleDamage(InEventData, GameplayEffectSpecHandle, TagSet);
}

void UHeroAbility_CounterAttack::ExecuteGameplayCueOnOnwer(FGameplayTag& InGameplayCueTag) const
{
	// Should make sure InGameplayCueTag is a GameplayCueTag

	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.NormalizedMagnitude = 1.f;
	GameplayCueParameters.Location = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(InGameplayCueTag, GameplayCueParameters);
}

FGameplayEffectSpecHandle UHeroAbility_CounterAttack::MakeAttackDamageSpecHandle(TSubclassOf<UGameplayEffect>& InDealDamageEffectClass, FGameplayTag& InSetByCallerAttackTypeTag, int32& InUsedAttackComboCount)
{
	float BaseWeaponDamage = GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel());
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeHeroDamageEffectsSpecHandle(
		InDealDamageEffectClass,
		BaseWeaponDamage,
		InSetByCallerAttackTypeTag,
		InUsedAttackComboCount
	);

	return GameplayEffectSpecHandle;
}

void UHeroAbility_CounterAttack::HandleDamage(FGameplayEventData& InEventData, FGameplayEffectSpecHandle& InGameplayEffectSpecHandle, FCounterAttackTagSet& InTagSet)
{
	AActor* TargetActor = const_cast<AActor*>(InEventData.Target.Get());
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InGameplayEffectSpecHandle);

	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, InTagSet.TargetHitReactEventTag, InEventData);
	}
}