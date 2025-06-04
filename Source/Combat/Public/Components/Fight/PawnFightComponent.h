// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"

#include "GameplayTagContainer.h"

#include "PawnFightComponent.generated.h"

class ACombatWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
	BothHand
};

/**
 * 
 */
UCLASS()
class COMBAT_API UPawnFightComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
public:
	// Function Bind To Delegate
	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, ACombatWeaponBase*> CharacterCarriedWeaponMap; // Contains all weapons character has

protected:
	TArray<AActor*> AttackOverlappedActors;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Fight")
	FGameplayTag CurrentEquippedWeaponTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Fight")
	ACombatWeaponBase* CurrentEquippedWeapon;

protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	virtual void ToggleBodyAttackCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);

public:
	UFUNCTION(BlueprintCallable, Category = "Combat|Fight")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, ACombatWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Combat|Fight")
	void SetCurrentEquippedWeaponAndTag(FGameplayTag InWeaponTag, ACombatWeaponBase* InWeapon);

	UFUNCTION(BlueprintCallable, Category = "Combat|Fight")
	void ClearCurrentEquippedWeaponAndTag();

	UFUNCTION(BlueprintCallable, Category = "Combat|Fight")
	ACombatWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Fight")
	ACombatWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Fight")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	
};
