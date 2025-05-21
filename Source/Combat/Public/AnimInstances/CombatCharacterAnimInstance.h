// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CombatBaseAnimInstance.h"
#include "CombatCharacterAnimInstance.generated.h"

class ACombatBaseCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatCharacterAnimInstance : public UCombatBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	ACombatBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;
};
