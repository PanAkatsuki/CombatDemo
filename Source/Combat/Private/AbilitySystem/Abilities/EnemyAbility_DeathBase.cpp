// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/EnemyAbility_DeathBase.h"

#include "Characters/CombatEnemyCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"


UEnemyAbility_DeathBase::UEnemyAbility_DeathBase()
{
	//Cast<ACombatEnemyCharacter>(GetAvatarActorFromActorInfo())->OnAsyncLoadFinishedDelegate.BindUObject()
}

void UEnemyAbility_DeathBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetPlayMontageTask(MontagesMap);
	ExecuteGameplayCueOnOnwer(DeathSoundGameplayCueTag);
}

void UEnemyAbility_DeathBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UEnemyAbility_DeathBase::SetPlayMontageTask(TMap<int32, UAnimMontage*>& InMontagesMap)
{
	check(MontagesMap.Num());

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayLightAttackMontageTask"),
		FindMontageToPlay(InMontagesMap),
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

UAnimMontage* UEnemyAbility_DeathBase::FindMontageToPlay(TMap<int32, UAnimMontage*>& InMontagesMap)
{
	int32 RandomInt = FMath::RandRange(1, InMontagesMap.Num());

	UAnimMontage* const* MontagePtr = InMontagesMap.Find(RandomInt);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UEnemyAbility_DeathBase::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEnemyAbility_DeathBase::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEnemyAbility_DeathBase::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UEnemyAbility_DeathBase::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UEnemyAbility_DeathBase::ExecuteGameplayCueOnOnwer(FGameplayTag& InGameplayCueTag) const
{
	// Should make sure InGameplayCueTag is a GameplayCueTag

	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.NormalizedMagnitude = 1.f;
	GameplayCueParameters.Location = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(InGameplayCueTag, GameplayCueParameters);
}