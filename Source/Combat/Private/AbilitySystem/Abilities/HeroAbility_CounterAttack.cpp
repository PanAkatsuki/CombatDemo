// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_CounterAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/CombatHeroCharacter.h"
#include "Components/Fight/HeroFightComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
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

	SetPlayMontageTask();
	SetWaitMontageEventTask();
}

void UHeroAbility_CounterAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetPawnFightComponentFromActorInfo()->ToggleWeaponCollision(false);
	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_CanCounterAttack);
	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_JumpToFinisher);
}

void UHeroAbility_CounterAttack::SetPlayMontageTask()
{
	check(MontagesMap.Num());

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayCounterAttackMontageTask"),
		FindMontageToPlay(),
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

UAnimMontage* UHeroAbility_CounterAttack::FindMontageToPlay()
{
	int32 RandomInt = FMath::RandRange(1, MontagesMap.Num());
	UAnimMontage* const* MontagePtr = MontagesMap.Find(RandomInt);

	return MontagePtr ? *MontagePtr : nullptr;
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

void UHeroAbility_CounterAttack::SetWaitMontageEventTask()
{
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CombatGameplayTags::Player_Event_CounterAttack,
		nullptr,
		false,
		true
	);

	WaitEventTask->EventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived);

	WaitEventTask->ReadyForActivation();
}

void UHeroAbility_CounterAttack::OnEventReceived(FGameplayEventData InEventData)
{
	ExecuteGameplayCueOnOnwer();

	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeAttackDamageSpecHandle();

	HandleDamage(InEventData, GameplayEffectSpecHandle);
}

void UHeroAbility_CounterAttack::ExecuteGameplayCueOnOnwer() const
{
	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.NormalizedMagnitude = 1.f;
	GameplayCueParameters.Location = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(TagSet.WeaponHitSoundGameplayCueTag, GameplayCueParameters);
}

FGameplayEffectSpecHandle UHeroAbility_CounterAttack::MakeAttackDamageSpecHandle()
{
	float BaseWeaponDamage = GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel());
	const int32 UsedComboCount = 1;
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeHeroDamageEffectsSpecHandle(
		DealDamageEffectClass,
		BaseWeaponDamage,
		CombatGameplayTags::Player_SetByCaller_AttackType_CounterAttack,
		UsedComboCount
	);

	return GameplayEffectSpecHandle;
}

void UHeroAbility_CounterAttack::HandleDamage(FGameplayEventData& InEventData, FGameplayEffectSpecHandle& InGameplayEffectSpecHandle)
{
	AActor* TargetActor = const_cast<AActor*>(InEventData.Target.Get());
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InGameplayEffectSpecHandle);

	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, CombatGameplayTags::Shared_Event_HitReact, InEventData);
	}
}