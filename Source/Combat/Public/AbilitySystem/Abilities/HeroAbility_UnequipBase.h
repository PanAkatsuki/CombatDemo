// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"

#include "Interfaces/AttachWeaponInterface.h"

#include "HeroAbility_UnequipBase.generated.h"

class ACombatHeroWeapon;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_UnequipBase : public UCombatHeroGameplayAbility, public IAttachWeaponInterface
{
	GENERATED_BODY()
	
public:
	UHeroAbility_UnequipBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	FName AttachSocketName;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//~ Begin IAttachWeaponInterface Interface ~//
	virtual void AttachWeaponToSocket(ACombatWeaponBase* InWeapon, USkeletalMeshComponent* InSkeletalMeshComponent, FName InSocketNameToAttachTo, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool WeldSimulatedBodies) override;
	//~ End IAttachWeaponInterface Interface ~//

private:
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
};
