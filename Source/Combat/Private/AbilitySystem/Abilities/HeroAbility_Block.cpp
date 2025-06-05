// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_Block.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CombatHeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatFunctionLibrary.h"

#include "CombatDebugHelper.h"


UHeroAbility_Block::UHeroAbility_Block()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Set LatentInfo
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnDelayFinished");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = 4;
}

void UHeroAbility_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(GetWorld());
	BlockActivateTime = UGameplayStatics::GetTimeSeconds(GetWorld());

	SetPlayMontageTask(MontageToPlay);

	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.TargetAttachComponent = GetOwningComponentFromActorInfo();
	K2_AddGameplayCueWithParams(TagSet.ShieldGameplayCueTag, GameplayCueParameters);

	SetWaitMontageEventTask(TagSet.WaitMontageEventTag);
}

void UHeroAbility_Block::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UHeroAbility_Block::SetPlayMontageTask(UAnimMontage* InMontageToPlay)
{
	check(InMontageToPlay);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayHeroBlockMontageTask"),
		InMontageToPlay,
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

void UHeroAbility_Block::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	//ResetGlobalTimeDilation();
}

void UHeroAbility_Block::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	//ResetGlobalTimeDilation();
}

void UHeroAbility_Block::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
	ResetGlobalTimeDilation();
}

void UHeroAbility_Block::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
	ResetGlobalTimeDilation();
}

void UHeroAbility_Block::ResetGlobalTimeDilation()
{
	check(GetWorld());
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void UHeroAbility_Block::SetWaitMontageEventTask(FGameplayTag& InWaitMontageEventTag)
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

void UHeroAbility_Block::OnEventReceived(FGameplayEventData InEventData)
{
	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.TargetAttachComponent = GetOwningComponentFromActorInfo();
	K2_ExecuteGameplayCueWithParams(TagSet.SuccessfulBlockGameplayCueTag, GameplayCueParameters);

	check(GetWorld());
	if (UGameplayStatics::GetTimeSeconds(GetWorld()) - BlockActivateTime <= PerfectBlockThreshold)
	{
		// Perfect Block
		UCombatFunctionLibrary::AddGameplayTagToActorIfNone(GetHeroCharacterFromActorInfo(), TagSet.PerfectBlockJumpToFinisherTag);
		UCombatFunctionLibrary::AddGameplayTagToActorIfNone(GetHeroCharacterFromActorInfo(), TagSet.PerfectBlockCounterAttackTag);
		K2_ExecuteGameplayCueWithParams(TagSet.PerfectBlockGameplayCueTag, GameplayCueParameters);

		check(GetWorld());
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);

		UKismetSystemLibrary::Delay(GetWorld(), 0.05f, LatentInfo);
	}
}

void UHeroAbility_Block::OnDelayFinished()
{
	check(GetWorld());
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	SetTimer(ResetCounterAttackTimerHandle, ResetCounterAttackTimerDelegate);
}

void UHeroAbility_Block::SetTimer(FTimerHandle& InTimerHandle, FTimerDelegate& InTimerDelegate)
{
	InTimerDelegate.BindUObject(this, &ThisClass::OnResetCounterAttackTimerFinished);

	GetWorld()->GetTimerManager().SetTimer(
		InTimerHandle,
		InTimerDelegate,
		0.3f,
		false
	);
}

void UHeroAbility_Block::OnResetCounterAttackTimerFinished()
{
	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), TagSet.PerfectBlockJumpToFinisherTag);
	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), TagSet.PerfectBlockCounterAttackTag);
}
