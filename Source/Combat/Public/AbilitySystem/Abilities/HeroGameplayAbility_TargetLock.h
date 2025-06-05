// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UCombatWidgetBase;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroGameplayAbility_TargetLock : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroGameplayAbility_TargetLock();

private:
	UPROPERTY(EditDefaultsOnly)
	float BoxTraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;
	
	UPROPERTY(EditDefaultsOnly)
	bool bShowPersistentDebugShape = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCombatWidgetBase> TargetLockWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	float TargetLockRotationInterpSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float TargetLockMaxWalkSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* TargetLockMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float TargetLockCameraOffsetDistance = 20.f;

	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	UPROPERTY()
	AActor* CurrentLockedActor = nullptr;

	UPROPERTY()
	UCombatWidgetBase* TargetLockWidget = nullptr;

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	UPROPERTY()
	float CachedDefaultMaxWalkSpeed = 0.f;

protected:
	//~ Begin UGameplayAbility Interface ~//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UgameplayAbility Interface ~//

	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);

private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);
	void DrawTargetLockWidget();
	void SetTargetLockWidgetPosition();
	void InitTargetLockMovement();
	void InitTargetLockMappingContext();

	void CancelTargetLockAbility();
	void CleanUp();
	void ClearAvailableActorsToLock();
	void ResetTargetLockMovement();
	void ResetTargetLockMappingContext();
};
