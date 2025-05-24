// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExecCalc_DamageTaken.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UGEExecCalc_DamageTaken : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEExecCalc_DamageTaken();

public:
	const float LightAttackDamageScaling = 1.0f;
	const float LightAttackComboDamageIncrease = 0.1f;

	const float HeavyAttackDamageScaling = 2.0f;
	const float HeavyAttackComboDamageIncrease = 0.5f;

	const float CounterAttackDamageScaling = 3.0f;
	const float CounterAttackComboDamageIncrease = 1.0f;

	const float SpecialAttackDamageScaling = 2.0f;
	const float SpecialAttackComboDamageIncrease = 1.0f;

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
