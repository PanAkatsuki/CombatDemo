// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_EquipBase.generated.h"

class ACombatHeroWeapon;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_EquipBase : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()

public:
	UHeroAbility_EquipBase();

protected:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketName;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagWeaponToEquipTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WaitMontageEventTag;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	void SetPlayMontageTask();
	void SetWaitMontageEventTask();
	void AttachWeapon();

protected:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

protected:
	ACombatHeroWeapon* GetCurrentEquippedWeapon();
	void HandleEquippedWeapon();
	void LinkAnimLayer();
	void AddMappingContext();
	void AssignWeaponAbilitySet();
	
};
