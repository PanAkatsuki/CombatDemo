// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_HeavyAttackBase.h"

#include "CombatFunctionLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "CombatGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/Fight/HeroFightComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

#include "CombatDebugHelper.h"


UHeroAbility_HeavyAttackBase::UHeroAbility_HeavyAttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_HeavyAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	InitializeTimer(AbilityEndTimerHandle);
	InitializeComboCount(CurrentAttackComboCount, UsedAttackComboCount);
	SetPlayMontageTask(AttackMontagesMap, CurrentAttackComboCount);
	SetWaitMontageEventTask(TagSet.WaitMontageEventTag);
	UpdateCurrentAttackComboCount(CurrentAttackComboCount);
}
void UHeroAbility_HeavyAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UHeroAbility_HeavyAttackBase::InitializeTimer(FTimerHandle& InTimerHandle)
{
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, InTimerHandle);
}

void UHeroAbility_HeavyAttackBase::InitializeComboCount(int32& InCurrentAttackComboCount, int32& InUsedAttackComboCount)
{
	//Debug::Print(TEXT("InitializeComboCount()CurrentAttackComboCount"), CurrentAttackComboCount);

	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_JumpToFinisher))
	{
		InCurrentAttackComboCount = AttackMontagesMap.Num();
	}

	InUsedAttackComboCount = InCurrentAttackComboCount;
}

void UHeroAbility_HeavyAttackBase::SetPlayMontageTask(TMap<int32, UAnimMontage*>& InMontagesMap, int32& InKey)
{
	check(AttackMontagesMap.Num());

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayLightAttackMontageTask"),
		FindMontageToPlay(InMontagesMap, InKey),
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

UAnimMontage* UHeroAbility_HeavyAttackBase::FindMontageToPlay(TMap<int32, UAnimMontage*>& InMontagesMap, int32& InKey)
{
	UAnimMontage* const* MontagePtr = InMontagesMap.Find(InKey);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UHeroAbility_HeavyAttackBase::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);
	SetTimer(AbilityEndTimerHandle, AbilityEndTimerDelegate);
}

void UHeroAbility_HeavyAttackBase::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);
	SetTimer(AbilityEndTimerHandle, AbilityEndTimerDelegate);
}

void UHeroAbility_HeavyAttackBase::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);
	SetTimer(AbilityEndTimerHandle, AbilityEndTimerDelegate);
}

void UHeroAbility_HeavyAttackBase::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);
	SetTimer(AbilityEndTimerHandle, AbilityEndTimerDelegate);
}

void UHeroAbility_HeavyAttackBase::SetTimer(FTimerHandle& InTimerHandle, FTimerDelegate& InTimerDelegate)
{
	InTimerDelegate.BindUObject(this, &ThisClass::OnAbilityEndTimerFinished);

	GetWorld()->GetTimerManager().SetTimer(
		InTimerHandle,
		InTimerDelegate,
		0.6f,
		false
	);
}

void UHeroAbility_HeavyAttackBase::OnAbilityEndTimerFinished()
{
	ResetCurrentAttackComboCount(CurrentAttackComboCount);
	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(CurrentActorInfo->AvatarActor.Get(), CombatGameplayTags::Player_Status_JumpToFinisher);
}

void UHeroAbility_HeavyAttackBase::SetWaitMontageEventTask(FGameplayTag& InEventTag)
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

void UHeroAbility_HeavyAttackBase::OnEventReceived(FGameplayEventData InEventData)
{
	ExecuteGameplayCueOnOnwer(TagSet.WeaponHitSoundGameplayCueTag);
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeAttackDamageSpecHandle(
		InEventData,
		EffectSet.DealDamageEffectClass,
		TagSet.SetByCallerAttackTypeTag,
		UsedAttackComboCount
	);
	HandleDamage(InEventData, GameplayEffectSpecHandle, TagSet, EffectSet.GainRageEffectClass);
}

void UHeroAbility_HeavyAttackBase::ExecuteGameplayCueOnOnwer(FGameplayTag& InGameplayCueTag) const
{
	// Should make sure InGameplayCueTag is a GameplayCueTag

	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.NormalizedMagnitude = 1.f;
	GameplayCueParameters.Location = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(InGameplayCueTag, GameplayCueParameters);
}

FGameplayEffectSpecHandle UHeroAbility_HeavyAttackBase::MakeAttackDamageSpecHandle(FGameplayEventData& InEventData, TSubclassOf<UGameplayEffect>& InDealDamageEffectClass, FGameplayTag& InSetByCallerAttackTypeTag, int32& InUsedAttackComboCount)
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

void UHeroAbility_HeavyAttackBase::HandleDamage(FGameplayEventData& InEventData, FGameplayEffectSpecHandle& InGameplayEffectSpecHandle, FHeavyAttackTagSet& InTagSet, TSubclassOf<UGameplayEffect>& InGainRageEffectClass)
{
	AActor* TargetActor = const_cast<AActor*>(InEventData.Target.Get());
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InGameplayEffectSpecHandle);

	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, InTagSet.TargetHitReactEventTag, InEventData);
		ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, InGainRageEffectClass.GetDefaultObject(), GetAbilityLevel());
	}
}

void UHeroAbility_HeavyAttackBase::UpdateCurrentAttackComboCount(int32& InCurrentAttackComboCount)
{
	if (InCurrentAttackComboCount == AttackMontagesMap.Num())
	{
		ResetCurrentAttackComboCount(InCurrentAttackComboCount);
	}
	else
	{
		++InCurrentAttackComboCount;
	}
}

void UHeroAbility_HeavyAttackBase::ResetCurrentAttackComboCount(int32& InCurrentAttackComboCount)
{
	InCurrentAttackComboCount = 1;
}