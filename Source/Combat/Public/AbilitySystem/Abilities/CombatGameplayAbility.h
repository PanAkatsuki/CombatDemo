// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CombatTypes/CombatEnumTypes.h"
#include "CombatGameplayAbility.generated.h"

class UPawnFightComponent;
class UCombatAbilitySystemComponent;

UENUM(BlueprintType)
enum class ECombatAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class COMBAT_API UCombatGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

private:
	// Cached Parameter
	TWeakObjectPtr<UCombatAbilitySystemComponent> CachedAbilitySystemComponent;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CombatAbility")
	ECombatAbilityActivationPolicy AbilityActivationPolicy = ECombatAbilityActivationPolicy::OnTriggered;

private:
	void TryActivateOnGivenAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);
	void ClearOnGivenAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo);

protected:
	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	UPawnFightComponent* GetPawnFightComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	UCombatAbilitySystemComponent* GetCombatAbilitySystemComponentFromActorInfo();

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);
	
	UFUNCTION(BlueprintCallable, Category = "Combat|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, ECombatSuccessType& OutSuccessType);

	UFUNCTION(BlueprintCallable, Category = "Combat|Ability")
	void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults);
};	
