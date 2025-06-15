// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroGameplayAbility_PickUpStones.generated.h"

class ACombatStoneBase;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroGameplayAbility_PickUpStones : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroGameplayAbility_PickUpStones();

private:
	UPROPERTY(EditDefaultsOnly)
	float BoxTraceDistance = 50.f;

	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(100.f);

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> StoneTraceChannel;

	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugShape = false;

	TArray<ACombatStoneBase*> CollectedStones;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	// Set Tick Task
	void SetTickTask();

	UFUNCTION()
	void OnTick(float DeltaTime);

	// Set Wait Event Task
	void SetWaitMontageEventTask();

	UFUNCTION()
	void OnEventReceived(FGameplayEventData InEventData);

	UFUNCTION(BlueprintCallable)
	void CollectStones();

	UFUNCTION(BlueprintCallable)
	void ConsumeStones();
};
