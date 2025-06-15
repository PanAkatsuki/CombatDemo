// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroGameplayAbility_PickUpStones.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "Items/PickUpItems/CombatStoneBase.h"
#include "Components/UI/HeroUIComponent.h"
#include "AbilitySystem/AbilityTasks/AbilityTask_ExecuteTaskOnTick.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"


UHeroGameplayAbility_PickUpStones::UHeroGameplayAbility_PickUpStones()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroGameplayAbility_PickUpStones::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(true);

	SetTickTask();
	SetWaitMontageEventTask();
}

void UHeroGameplayAbility_PickUpStones::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(false);
}

void UHeroGameplayAbility_PickUpStones::SetTickTask()
{
	UAbilityTask_ExecuteTaskOnTick* TickTask = UAbilityTask_ExecuteTaskOnTick::ExecuteTasksOnTick(this);
	check(TickTask);

	TickTask->OnAbilityTaskTick.AddDynamic(this, &ThisClass::OnTick);

	TickTask->ReadyForActivation();
}

void UHeroGameplayAbility_PickUpStones::OnTick(float DeltaTime)
{
	//Debug::Print(TEXT("Tick"));
	CollectStones();
}

void UHeroGameplayAbility_PickUpStones::SetWaitMontageEventTask()
{
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CombatGameplayTags::Player_Event_ConsumeStones,
		nullptr,
		false,
		true
	);

	WaitEventTask->EventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived);

	WaitEventTask->ReadyForActivation();
}

void UHeroGameplayAbility_PickUpStones::OnEventReceived(FGameplayEventData InEventData)
{
	//Debug::Print(TEXT("Consume"));
	ConsumeStones();
}

void UHeroGameplayAbility_PickUpStones::CollectStones()
{
	ACombatHeroCharacter* HeroCharacter = Cast<ACombatHeroCharacter>(CurrentActorInfo->AvatarActor.Get());

	check(HeroCharacter);

	CollectedStones.Empty();

	TArray<FHitResult> TraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		HeroCharacter,
		HeroCharacter->GetActorLocation(),
		HeroCharacter->GetActorLocation() - HeroCharacter->GetActorUpVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		(-HeroCharacter->GetActorUpVector() * BoxTraceDistance).ToOrientationRotator(),
		StoneTraceChannel,
		false,
		TArray<AActor*>(),
		bDrawDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		TraceHits,
		true
	);

	for (const FHitResult& TraceHit : TraceHits)
	{
		if (ACombatStoneBase* FoundStone = Cast<ACombatStoneBase>(TraceHit.GetActor()))
		{
			CollectedStones.AddUnique(FoundStone);
		}
	}

	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UHeroGameplayAbility_PickUpStones::ConsumeStones()
{
	if (CollectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}

	for (ACombatStoneBase* CollectedStone : CollectedStones)
	{
		if (CollectedStone)
		{
			CollectedStone->Consume(GetHeroCharacterFromActorInfo()->GetCombatAbilitySystemComponent(), GetAbilityLevel());
		}
	}
}