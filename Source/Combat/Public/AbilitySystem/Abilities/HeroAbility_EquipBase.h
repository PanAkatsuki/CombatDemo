// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_EquipBase.generated.h"

class ACombatHeroWeapon;

USTRUCT(BlueprintType)
struct FEquipTagSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponToEquipTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WaitMontageEventTag;
};

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
	FEquipTagSet TagSet;

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketName;
	
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
	void HandleEquippedWeapon();
	void AttachWeapon(FGameplayTag& InWeaponToEquipTag, FName& InAttachSocketName);
	void RegisterCurrentEquippedWeapon(FGameplayTag& InWeaponToRegister);
	void LinkAnimLayer();
	void AddMappingContext();
	void AssignWeaponAbilitySet();
	void UpdateUISystem();
};
