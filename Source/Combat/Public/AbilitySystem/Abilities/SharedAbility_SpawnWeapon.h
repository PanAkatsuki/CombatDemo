// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatGameplayAbility.h"
#include "SharedAbility_SpawnWeapon.generated.h"

class ACombatWeaponBase;

/**
 * 
 */
UCLASS()
class COMBAT_API USharedAbility_SpawnWeapon : public UCombatGameplayAbility
{
	GENERATED_BODY()

public:
	USharedAbility_SpawnWeapon();

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACombatWeaponBase> WeaponToSpawn;

	UPROPERTY(EditDefaultsOnly)
	FName SocketNameToAttachTo;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponTagToRigister;

	UPROPERTY(EditDefaultsOnly)
	bool RegisterAsEquippedWeapon;
};
