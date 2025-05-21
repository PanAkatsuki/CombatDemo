// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UCombatGameplayAbility;
class UCombatAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class COMBAT_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UCombatGameplayAbility>> ActivateOnGivenAbilitieSet;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UCombatGameplayAbility>> ReactiveAbilitieSet;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffectSet;

protected:
	void GrantAbilities(const TArray<TSubclassOf<UCombatGameplayAbility>>& InAbilitiesToGive, UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
	void GrantEffects(const TArray<TSubclassOf<UGameplayEffect>>& InEffectsToGive, UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

public:
	virtual void GiveDataToAbilitySystemComponent(UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

};
