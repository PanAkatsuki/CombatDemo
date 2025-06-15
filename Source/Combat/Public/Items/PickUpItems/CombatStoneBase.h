// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUpItems/CombatPickUpItemBase.h"
#include "CombatStoneBase.generated.h"

class UCombatAbilitySystemComponent;
class UGameplayEffect;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatStoneBase : public ACombatPickUpItemBase
{
	GENERATED_BODY()

public:
	ACombatStoneBase();

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UNiagaraSystem> ConsumeFX;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ConsumeSound;

public:
	void Consume(UCombatAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

protected:
	//~ Begin ACombatPickUpItemBase Interface ~//
	virtual void OnPickUpZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnPickUpZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//~ End ACombatPickUpItemBase Interface ~//

	void OnStoneConsumed();

	UFUNCTION()
	void OnAsyncLoadConsumeFXFinished();
};
