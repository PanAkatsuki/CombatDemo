// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_Roll.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionWarpingComponent.h"

#include "CombatDebugHelper.h"


UHeroAbility_Roll::UHeroAbility_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Set LatentInfo
	RollLatentInfo.CallbackTarget = this;
	RollLatentInfo.ExecutionFunction = FName("OnDelayFinished");
	RollLatentInfo.Linkage = 0;
	RollLatentInfo.UUID = GetTypeHash(FName("OnDelayFinished"));
}

void UHeroAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(GetWorld());
	UKismetSystemLibrary::Delay(GetWorld(), 0.05f, RollLatentInfo);
}

void UHeroAbility_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UHeroAbility_Roll::OnDelayFinished()
{
	ComputeRollDiractionAndDistance();
	SetPlayMontageTask(this, FName("RollMontageTask"), FindMontageToPlayByRandom(AnimMontagesMap));
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