// Zhang All Rights Reserved.


#include "AbilitySystem/CombatAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/CombatGameplayAbility.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"

void UCombatAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!Spec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
		{
			continue;
		}

		// Toggleable Abilities
		if (InInputTag.MatchesTag(CombatGameplayTags::InputTag_Toggleable))
		{
			if (Spec.IsActive())
			{
				CancelAbilityHandle(Spec.Handle);
			}
			else
			{
				bool bSuccessfulActivate = TryActivateAbility(Spec.Handle);

				// Debug
				/*if (bSuccessfulActivate)
				{
					Debug::Print(TEXT("Activate Successed!"));
				}
				else
				{
					Debug::Print(TEXT("Activate Failed!"));
				}*/
			}
		}
		else
		{
			// Other Abilities
			TryActivateAbility(Spec.Handle);
		}
	}
}

void UCombatAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	// InputTag is not vaild or Abilities are NOT MustBeHeld
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(CombatGameplayTags::InputTag_MustBeHeld))
	{
		return;
	}

	// InputTag_MustBeHeld
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && Spec.IsActive())
		{
			CancelAbilityHandle(Spec.Handle);
		}
	}
}

void UCombatAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FCombatHeroAbilitySets>& InDefaultWeaponAbilities, const TArray<FCombatHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedABilitySpecHandles)
{
	for (const FCombatHeroAbilitySets& DefaultWeaponAbility : InDefaultWeaponAbilities)
	{
		if (!DefaultWeaponAbility.IsValid())
		{
			continue;
		}

		// Set GameplayAbilitySpec
		FGameplayAbilitySpec Spec(DefaultWeaponAbility.AbilityToGrant);
		Spec.SourceObject = GetAvatarActor();
		Spec.Level = ApplyLevel;
		Spec.GetDynamicSpecSourceTags().AddTag(DefaultWeaponAbility.InputTag);

		FGameplayAbilitySpecHandle NewAbilitySpecHandle = GiveAbility(Spec);
		OutGrantedABilitySpecHandles.AddUnique(NewAbilitySpecHandle);
	}
	
	for (const FCombatHeroSpecialAbilitySet& SpecialWeaponAbility : InSpecialWeaponAbilities)
	{
		if (!SpecialWeaponAbility.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec Spec(SpecialWeaponAbility.AbilityToGrant);
		Spec.SourceObject = GetAvatarActor();
		Spec.Level = ApplyLevel;
		Spec.GetDynamicSpecSourceTags().AddTag(SpecialWeaponAbility.InputTag);

		FGameplayAbilitySpecHandle NewAbilitySpecHandle = GiveAbility(Spec);
		OutGrantedABilitySpecHandles.AddUnique(NewAbilitySpecHandle);
	}
}

void UCombatAbilitySystemComponent::RemoveGrantHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InAbilitySpecHandle)
{
	if (InAbilitySpecHandle.IsEmpty())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : InAbilitySpecHandle)
	{
		if (AbilitySpecHandle.IsValid())
		{
			ClearAbility(AbilitySpecHandle);
		}
	}

	InAbilitySpecHandle.Empty();
}

bool UCombatAbilitySystemComponent::TryAcitivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	TArray<FGameplayAbilitySpec*> AbilitySpec;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), AbilitySpec);

	if (!AbilitySpec.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, AbilitySpec.Num() - 1);
		FGameplayAbilitySpec* AbilitySpecToActivate = AbilitySpec[RandomAbilityIndex];

		check(AbilitySpecToActivate);

		if (!AbilitySpecToActivate->IsActive())
		{
			return TryActivateAbility(AbilitySpecToActivate->Handle);
		}
	}

	return false;
}
