// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_UnequipBase.generated.h"

class ACombatHeroWeapon;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_UnequipBase : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_UnequipBase();

protected:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketName;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WaitMontageEventTag;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	void SetPlayMontageTask(UAnimMontage* InMontageToPlay);
	
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();

	void SetWaitMontageEventTask(FGameplayTag& InWaitMontageEventTag);

	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	ACombatHeroWeapon* GetCurrentEquippedWeapon();
	void HandleUnequippedWeapon();
	void AttachWeapon(FName& InAttachSocketName);
	void UnlinkAnimLayer();
	void RemoveMappingContext();
	void RemoveWeaponAbilitySet();
	void UpdateUISystem();
	void DeleteRegisterInfo();
};
