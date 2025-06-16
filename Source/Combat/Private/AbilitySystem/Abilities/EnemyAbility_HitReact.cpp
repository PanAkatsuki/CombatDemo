// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/EnemyAbility_HitReact.h"

#include "Kismet/KismetMathLibrary.h"
#include "Characters/CombatEnemyCharacter.h"
#include "CombatGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

#include "CombatDebugHelper.h"


UEnemyAbility_HitReact::UEnemyAbility_HitReact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Set AbilityTriggerData
	FAbilityTriggerData AbilityTriggerData = FAbilityTriggerData();
	AbilityTriggerData.TriggerTag = CombatGameplayTags::Shared_Event_HitReact;
	AbilityTriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(AbilityTriggerData);

	// Set LatentInfo
	HitReactLatentInfo.CallbackTarget = this;
	HitReactLatentInfo.ExecutionFunction = FName("OnHitReactDelayFinished");
	HitReactLatentInfo.Linkage = 0;
	HitReactLatentInfo.UUID = GetTypeHash(FName("OnHitPauseDelayFinished"));
}

void UEnemyAbility_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	MakeEnemyFaceToAttacker(*TriggerEventData);

	if (DoesAbilityHaveMontagesToPlay())
	{
		SetPlayMontageTask(this, FName("PlayEnemyHitReactMontageTask"), FindMontageToPlayByRandom(AnimMontagesMap));
		GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.0f);
	}
	else
	{
		GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1.0f);
		check(GetWorld());
		UKismetSystemLibrary::Delay(GetWorld(), 0.2f, HitReactLatentInfo);
	}

	ApplyGameplayEffectSpecToOwner(
		Handle,
		ActorInfo,
		ActivationInfo,
		MakeOutgoingGameplayEffectSpec(UnderAttackEffect)
	);
}

void UEnemyAbility_HitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 0.0f);
}

void UEnemyAbility_HitReact::MakeEnemyFaceToAttacker(const FGameplayEventData& EventData)
{
	if (!bFaceToAttacker)
	{
		return;
	}

	ACombatEnemyCharacter* Enemy = GetEnemyCharacterFromActorInfo();

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), EventData.Instigator->GetActorLocation());

	Enemy->SetActorRotation(NewRotation);
}

bool UEnemyAbility_HitReact::DoesAbilityHaveMontagesToPlay()
{
	return AnimMontagesMap.Num() > 0 ? true : false;
}

void UEnemyAbility_HitReact::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEnemyAbility_HitReact::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEnemyAbility_HitReact::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UEnemyAbility_HitReact::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UEnemyAbility_HitReact::OnHitReactDelayFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}