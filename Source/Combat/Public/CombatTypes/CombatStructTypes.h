// Zhang All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"

#include "CombatStructTypes.generated.h"

class UCombatHeroGameplayAbility;
class UCombatHeroLinkedAnimLayer;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FCombatHeroAbilitySets
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCombatHeroGameplayAbility> AbilityToGrant;

public:
	bool IsValid() const;

};

USTRUCT(BlueprintType)
struct FCombatHeroSpecialAbilitySet : public FCombatHeroAbilitySets
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> SoftAbilityIconMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Player.Cooldown"))
	FGameplayTag AbilityCooldownTag;
};

USTRUCT(BlueprintType)
struct FCombatHeroWeaponData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCombatHeroLinkedAnimLayer> WeaponAnimLayerToLink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FCombatHeroAbilitySets> DefaultWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FCombatHeroSpecialAbilitySet> SpecialWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};

class COMBAT_API CombatStructTypes
{
public:
	CombatStructTypes() = default;
	~CombatStructTypes() = default;
};
