// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_Rage.h"

#include "CombatFunctionLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "CombatGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

#include "CombatDebugHelper.h"


UHeroAbility_Rage::UHeroAbility_Rage()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_Rage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UCombatFunctionLibrary::AddGameplayTagToActorIfNone(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_Rage_Activating);

	SetPlayMontageTask(this, FName("RageMontageTask"), FindMontageToPlay(AnimMontagesMap));
	SetWaitMontageEventTask(this, CombatGameplayTags::Player_Event_ActivateRage);

	// Wait Tag Added Task
	UAbilityTask_WaitGameplayTagAdded* WaitTagAddedTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(
		this,
		CombatGameplayTags::Player_Status_Rage_None
	);

	WaitTagAddedTask->Added.AddUniqueDynamic(this, &ThisClass::OnTagAdded);

	WaitTagAddedTask->ReadyForActivation();

	// Apply Gameplay Effect To Owner
	AppliedRageCostGameplayEffectHandle = BP_ApplyGameplayEffectToOwner(RageCostEffectClass, GetAbilityLevel());
}

void UHeroAbility_Rage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	BP_RemoveGameplayEffectFromOwnerWithHandle(AppliedRageCostGameplayEffectHandle);
}

void UHeroAbility_Rage::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);

	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_Rage_Activating);
}

void UHeroAbility_Rage::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);

	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_Rage_Activating);
}

void UHeroAbility_Rage::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);

	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_Rage_Activating);
}

void UHeroAbility_Rage::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);

	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_Rage_Activating);
}

void UHeroAbility_Rage::OnEventReceived(FGameplayEventData InEventData)
{
	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.TargetAttachComponent = GetOwningComponentFromActorInfo();
	K2_AddGameplayCueWithParams(TagSet.RageGameplayCueTag, GameplayCueParameters);
}

void UHeroAbility_Rage::OnTagAdded()
{
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
}