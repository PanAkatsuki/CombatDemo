// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_Roll.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionWarpingComponent.h"

#include "CombatDebugHelper.h"


UHeroAbility_Roll::UHeroAbility_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Set LatentInfo
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnDelayFinished");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = 4;
}

void UHeroAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(GetWorld());
	UKismetSystemLibrary::Delay(GetWorld(), 0.05f, LatentInfo);
}

void UHeroAbility_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UHeroAbility_Roll::OnDelayFinished()
{
	ComputeRollDiractionAndDistance();
	SetPlayMontageTask(MontageToPlay);
}

void UHeroAbility_Roll::ComputeRollDiractionAndDistance()
{
	FVector RollingDirection = UKismetMathLibrary::Normal(GetHeroCharacterFromActorInfo()->GetLastMovementInputVector());
	GetHeroCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("RollingDirection"),
		FVector(),
		UKismetMathLibrary::MakeRotFromX(RollingDirection)
	);

	//
	check(GetWorld());

	FVector HeroLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	float RollingDistance = RollDistanceScalableFloat.GetValueAtLevel(GetAbilityLevel());
	FVector StartVector = HeroLocation + UKismetMathLibrary::Multiply_VectorFloat(RollingDirection, RollingDistance);

	FVector EndVector = GetHeroCharacterFromActorInfo()->GetActorUpVector() * -1.f * 500.f + StartVector;

	TArray<FHitResult> OutHits;

	UKismetSystemLibrary::LineTraceMultiForObjects(
		GetWorld(),
		StartVector,
		EndVector,
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

void UHeroAbility_Roll::SetPlayMontageTask(UAnimMontage* InMontageToPlay)
{
	check(InMontageToPlay);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName("PlayHeroRollMontageTask"),
		InMontageToPlay,
		1.0f
	);

	PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageCancelled);

	PlayMontageTask->ReadyForActivation();
}

void UHeroAbility_Roll::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_Roll::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_Roll::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_Roll::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}