// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUpItems/CombatPickUpItemBase.h"
#include "CombatStoneBase.generated.h"

class UCombatAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatStoneBase : public ACombatPickUpItemBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;

protected:
	virtual void OnPickUpZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnPickUpZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();

public:
	void Consume(UCombatAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

};
