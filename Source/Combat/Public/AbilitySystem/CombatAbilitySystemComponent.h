// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "CombatTypes/CombatStructTypes.h"

#include "CombatAbilitySystemComponent.generated.h"


/**
 * 
 */
UCLASS()
class COMBAT_API UCombatAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "Combat|Ability", meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FCombatHeroAbilitySets>& InDefaultWeaponAbilities, const TArray<FCombatHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedABilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "Combat|Ability")
	void RemoveGrantHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InAbilitySpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Combat|Ability")
	bool TryAcitivateAbilityByTag(FGameplayTag AbilityTagToActivate);

}; 
