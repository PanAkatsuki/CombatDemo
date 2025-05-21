// Zhang All Rights Reserved.


#include "AbilitySystem/Abilities/CombatGameplayAbility.h"

#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "Components/Fight/PawnFightComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CombatFunctionLibrary.h"
#include "CombatGameplayTags.h"


void UCombatGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	TryActivateOnGivenAbility(ActorInfo, Spec);
}

void UCombatGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ClearOnGivenAbility(Handle, ActorInfo);
}

void UCombatGameplayAbility::TryActivateOnGivenAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (AbilityActivationPolicy == ECombatAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UCombatGameplayAbility::ClearOnGivenAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo)
{
	if (AbilityActivationPolicy == ECombatAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnFightComponent* UCombatGameplayAbility::GetPawnFightComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnFightComponent>();
}

UCombatAbilitySystemComponent* UCombatGameplayAbility::GetCombatAbilitySystemComponentFromActorInfo()
{
	if (!CachedAbilitySystemComponent.IsValid())
	{
		CachedAbilitySystemComponent = Cast<UCombatAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
	}

	return CachedAbilitySystemComponent.IsValid() ? CachedAbilitySystemComponent.Get() : nullptr;
}

FActiveGameplayEffectHandle UCombatGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	checkf(TargetASC, TEXT("TargetASC is not valid."));
	checkf(InSpecHandle.IsValid(), TEXT("InSpecHandle is not valid."));

	return GetCombatAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UCombatGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, ECombatSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? ECombatSuccessType::Successful : ECombatSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}

void UCombatGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults)
{
	if (InHitResults.IsEmpty())
	{
		return;
	}

	for (const FHitResult& hit : InHitResults)
	{
		APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());
		// HitPawn can be actor can't cast to APawn, ensure check if cast success
		APawn* HitPawn = Cast<APawn>(hit.GetActor());

		if (HitPawn && UCombatFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
		{
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(hit.GetActor(), InSpecHandle);

			if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
			{
				FGameplayEventData Data;
				Data.Instigator = OwningPawn;
				Data.Target = HitPawn;

				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(hit.GetActor(), CombatGameplayTags::Shared_Event_HitReact, Data);
			}
		}
	}
}

