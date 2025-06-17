// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatHeroGameplayAbility.h"
#include "HeroAbility_Aim.generated.h"

class UCombatWidgetBase;
class ACombatEnemyCharacter;
struct FWidgetTransform;

/**
 * 
 */
UCLASS()
class COMBAT_API UHeroAbility_Aim : public UCombatHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UHeroAbility_Aim();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TSubclassOf<UCombatWidgetBase> CrosshairWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Ability")
	TArray<TEnumAsByte<EObjectTypeQuery>> LineTraceChannel;

	UCombatWidgetBase* CrosshairWidget = nullptr;
	TArray<UCombatWidgetBase*> TargetInWidget;
	FVector2D CrosshairWidgetSize = FVector2D::ZeroVector;
	TWeakObjectPtr<ACombatEnemyCharacter> CachedTraceEnemyCharacter;

	FTimerHandle EnemyDetectTimerHandle;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void CreateCrosshairAndSetPosition();
	void DetectEnemyInCrosshair();
	FVector2D GetScreenCenterPosition();
	void AddTargetInWidgetToViewport(UCombatWidgetBase* InWidget, const FWidgetTransform& InFlipTransform, const FVector2D& InOffset);
	void ClearTracedEnemy();
	void ClearTargetInWidget();

	// Callback funciton for play montage task
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageBlendOut();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageCancelled();
};
