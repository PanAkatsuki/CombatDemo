// Zhang All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"

#include "AbilitySystem/Abilities/CombatGameplayAbility.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

void UDataAsset_HeroStartUpData::GiveDataToAbilitySystemComponent(UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    Super::GiveDataToAbilitySystemComponent(InASCToGive, ApplyLevel);

    if (HeroStartUpAbilitySet.IsEmpty())
    {
        return;
    }

    for (const FCombatHeroAbilitySets& Ability : HeroStartUpAbilitySet)
    {
        if (!Ability.IsValid())
        {
            continue;
        }

        FGameplayAbilitySpec AbilitySpec(Ability.AbilityToGrant, ApplyLevel, -1, InASCToGive->GetAvatarActor());
        AbilitySpec.GetDynamicSpecSourceTags().AddTag(Ability.InputTag);

        InASCToGive->GiveAbility(AbilitySpec);
    }
}
