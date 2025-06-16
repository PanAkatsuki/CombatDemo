// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroAbility_Block.h"

#include "Kismet/GameplayStatics.h"
#include "Characters/CombatHeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatFunctionLibrary.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"


UHeroAbility_Block::UHeroAbility_Block()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Set LatentInfo
	BlockLatentInfo.CallbackTarget = this;
	BlockLatentInfo.ExecutionFunction = FName("OnDelayFinished");
	BlockLatentInfo.Linkage = 0;
	BlockLatentInfo.UUID = GetTypeHash(FName("OnDelayFinished"));
}

void UHeroAbility_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(GetWorld());
	BlockActivateTime = UGameplayStatics::GetTimeSeconds(GetWorld());

	SetPlayMontageTask(this, FName("BlockMontageTask"), FindMontageToPlayByRandom(AnimMontagesMap));

	FGameplayCueParameters GameplayCueParameters;
	GameplayCueParameters.TargetAttachComponent = GetOwningComponentFromActorInfo();
	K2_AddGameplayCueWithParams(TagSet.ShieldGameplayCueTag, GameplayCueParameters);

	SetWaitMontageEventTask(this, CombatGameplayTags::Player_Event_SuccessfulBlock, nullptr, false, false);
}

void UHeroAbility_Block::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ResetGlobalTimeDilation();
}

void UHeroAbility_Block::OnMontageCompleted()
{
	//Debug::Print(TEXT("MontageCompleted"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_Block::OnMontageBlendOut()
{
	//Debug::Print(TEXT("MontageBlendOut"));
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_Block::OnMontageInterrupted()
{
	//Debug::Print(TEXT("MontageInterrupted"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UHeroAbility_Block::OnMontageCancelled()
{
	//Debug::Print(TEXT("MontageCancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
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
		UCombatFunctionLibrary::AddGameplayTagToActorIfNone(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_JumpToFinisher);
		UCombatFunctionLibrary::AddGameplayTagToActorIfNone(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_CanCounterAttack);
		K2_ExecuteGameplayCueWithParams(TagSet.PerfectBlockGameplayCueTag, GameplayCueParameters);

		check(GetWorld());
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);

		UKismetSystemLibrary::Delay(GetWorld(), 0.05f, BlockLatentInfo);
	}
}

void UHeroAbility_Block::OnDelayFinished()
{
	check(GetWorld());
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	ResetCounterAttackTimerDelegate.BindUObject(this, &ThisClass::OnResetCounterAttackTimerFinished);

	GetWorld()->GetTimerManager().SetTimer(
		ResetCounterAttackTimerHandle,
		ResetCounterAttackTimerDelegate,
		0.3f,
		false
	);
}

void UHeroAbility_Block::OnResetCounterAttackTimerFinished()
{
	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_JumpToFinisher);
	UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetHeroCharacterFromActorInfo(), CombatGameplayTags::Player_Status_CanCounterAttack);
}

void UHeroAbility_Block::ResetGlobalTimeDilation()
{
	check(GetWorld());
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}