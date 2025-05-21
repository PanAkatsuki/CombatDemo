// Zhang All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"

#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/CombatEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveDataToAbilitySystemComponent(UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveDataToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if (EnemyFightAbilities.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UCombatEnemyGameplayAbility>& AbilityClass : EnemyFightAbilities)
	{
		if (!AbilityClass)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilityClass, ApplyLevel, -1, InASCToGive->GetAvatarActor());

		InASCToGive->GiveAbility(AbilitySpec);
	}
}
