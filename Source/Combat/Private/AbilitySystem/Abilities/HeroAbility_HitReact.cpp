// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_HitReact.h"

#include "CombatGameplayTags.h"
#include "CombatFunctionLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "CombatDebugHelper.h"


UHeroAbility_HitReact::UHeroAbility_HitReact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Set AbilityTriggerData
	FAbilityTriggerData AbilityTriggerData = FAbilityTriggerData();
	AbilityTriggerData.TriggerTag = CombatGameplayTags::Shared_Event_HitReact;
	AbilityTriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(AbilityTriggerData);
}

void UHeroAbility_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	float AngleDifference;
	FGameplayTag HitReactDirectionTag = UCombatFunctionLibrary::ComputeHitReactDirectionTag(
		TriggerEventData->Instigator.Get(),
		GetHeroCharacterFromActorInfo(),
		AngleDifference
	);

	int32 MontageKey = 0;
	if (HitReactDirectionTag == CombatGameplayTags::Shared_Status_HitReact_Front)
	{
		MontageKey = 1;
	}
	else if (HitReactDirectionTag == CombatGameplayTags::Shared_Status_HitReact_Back)
	{
		MontageKey = 2;
	}
	else if (HitReactDirectionTag == CombatGameplayTags::Shared_Status_HitReact_Right)
	{
		MontageKey = 3;
	}
	else if (HitReactDirectionTag == CombatGameplayTags::Shared_Status_HitReact_Left)
	{
		MontageKey = 4;
	}

	check(MontageKey);
	SetPlayMontageTask(MontagesMap, MontageKey);

	GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.0f);
}

void UHeroAbility_HitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 0.0f);
}


void UHeroAbility_HitReact::SetPlayMontageTask(TMap<int32, UAnimMontage*>& InMontagesMap, int32 InKey)
{
	check(InMontagesMap.Num());

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayHeroHitReactMontageTask"),
		FindMontageToPlay(InMontagesMap, InKey),
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

UAnimMontage* UHeroAbility_HitReact::FindMontageToPlay(TMap<int32, UAnimMontage*>& InMontagesMap, int32 InKey)
{
	UAnimMontage* const* MontagePtr = InMontagesMap.Find(InKey);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UHeroAbility_HitReact::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_HitReact::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_HitReact::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_HitReact::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}