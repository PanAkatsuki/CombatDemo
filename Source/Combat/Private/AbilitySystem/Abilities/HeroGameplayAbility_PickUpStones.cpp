// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/HeroGameplayAbility_PickUpStones.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Characters/CombatHeroCharacter.h"
#include "Items/PickUpItems/CombatStoneBase.h"
#include "Components/UI/HeroUIComponent.h"


void UHeroGameplayAbility_PickUpStones::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(true);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_PickUpStones::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//CollectedStones.Empty();
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
