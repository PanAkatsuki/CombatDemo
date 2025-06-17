// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_TimeCounter.h"

#include "Characters/CombatHeroCharacter.h"
#include "Characters/CombatEnemyCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "CombatFunctionLibrary.h"
#include "CombatGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/Fight/HeroFightComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "CombatDebugHelper.h"


UHeroAbility_TimeCounter::UHeroAbility_TimeCounter()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_TimeCounter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UCombatFunctionLibrary::AddGameplayTagToActorIfNone(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Shared_Status_Invincible);

	if (GetHeroCharacterFromActorInfo()->TargetInEnemyCharacter.IsValid())
	{
		//Debug::Print(TEXT("GetEnemy"));
		ACombatEnemyCharacter* EnemyCharacter = GetHeroCharacterFromActorInfo()->TargetInEnemyCharacter.Get();
		float Distance = FVector::Dist(GetHeroCharacterFromActorInfo()->GetActorLocation(), EnemyCharacter->GetActorLocation());
		if (Distance <= TimeCounterDistance)
		{
			//Debug::Print(TEXT("TimeCOunter"));
			SetPlayMontageTask(this, FName("TimeCounterMontageTask"), FindMontageToPlayWithKey(AnimMontagesMap, 2));

			SetWaitMontageEventTask(this, CombatGameplayTags::Player_Event_TimeCounter);
		}
		else
		{
			//Debug::Print(TEXT("PlayRoll"));
			FindNearestPointAroundTargetAndSetMotionWarping(GetHeroCharacterFromActorInfo(), EnemyCharacter, TimeCounterDistance);

			UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				FName("TimeCounterRollMontageTask"),
				FindMontageToPlayWithKey(AnimMontagesMap, 1)
			);

			PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnRollCompleted);

			PlayMontageTask->ReadyForActivation();
		}
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
}

void UHeroAbility_TimeCounter::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Shared_Status_Invincible);
}

UAnimMontage* UHeroAbility_TimeCounter::FindMontageToPlayWithKey(TMap<int32, UAnimMontage*>& InAnimMontagesMap, int32 InKey)
{
	UAnimMontage* const* MontagePtr = AnimMontagesMap.Find(InKey);

	return MontagePtr ? *MontagePtr : nullptr;
}

void UHeroAbility_TimeCounter::FindNearestPointAroundTargetAndSetMotionWarping(AActor* SelfActor, AActor* TargetActor, float DesiredDistance)
{
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector SelfLocation = SelfActor->GetActorLocation();

	FVector Direction = (TargetLocation - SelfLocation).GetSafeNormal();
	FVector DesiredLocation = TargetLocation - Direction * DesiredDistance;

	GetHeroCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("RollingDirection"),
		FVector(),
		UKismetMathLibrary::MakeRotFromX(Direction)
	);

	TArray<FHitResult> OutHits;

	UKismetSystemLibrary::LineTraceMultiForObjects(
		GetWorld(),
		DesiredLocation + FVector(0.f, 0.f, 500.f),
		DesiredLocation + FVector(0.f, 0.f, -500.f),
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		OutHits,
		true
	);

	check(OutHits.Num());
	if (OutHits.GetData()->bBlockingHit)
	{
		GetHeroCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			FName("RollTargetLocation"),
			OutHits.GetData()->ImpactPoint
		);
	}
	else
	{
		GetHeroCharacterFromActorInfo()->GetMotionWarpingComponent()->RemoveWarpTarget(
			FName("RollTargetLocation")
		);
	}
}

void UHeroAbility_TimeCounter::OnRollCompleted()
{
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("TimeCounterAttackMontageTask"),
		FindMontageToPlayWithKey(AnimMontagesMap, 2)
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);

	PlayMontageTask->ReadyForActivation();

	SetWaitMontageEventTask(this, CombatGameplayTags::Player_Event_TimeCounter);
}

void UHeroAbility_TimeCounter::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_TimeCounter::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_TimeCounter::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_TimeCounter::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_TimeCounter::OnEventReceived(FGameplayEventData InEventData)
{
	//Debug::Print(TEXT("Hit"));
	// Play Gameplay Cue
	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.NormalizedMagnitude = 1.f;
	GameplayCueParameters.Location = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

	CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(TagSet.WeaponHitSoundGameplayCueTag, GameplayCueParameters);

	// Make Damage Spec
	float BaseWeaponDamage = GetHeroFightComponentFromActorInfo()->GetHeroCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel());
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeHeroDamageEffectsSpecHandle(
		EffectSet.DealDamageEffectClass,
		BaseWeaponDamage,
		CombatGameplayTags::Player_SetByCaller_AttackType_SpecialWeaponAbility,
		1
	);

	AActor* TargetActor = const_cast<AActor*>(InEventData.Target.Get());
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, GameplayEffectSpecHandle);

	if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, CombatGameplayTags::Shared_Event_HitReact, InEventData);

		ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSet.GainRageEffectClass.GetDefaultObject(), GetAbilityLevel());
	}
}
