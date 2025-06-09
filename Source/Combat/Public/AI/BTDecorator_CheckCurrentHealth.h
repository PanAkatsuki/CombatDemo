// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "AttributeSet.h"

#include "BTDecorator_CheckCurrentHealth.generated.h"


UENUM(BlueprintType)
enum class EHealthCompareMode : uint8
{
	LessThan			UMETA(DisplayName = "Less Than"),
	LessThanOrEqual		UMETA(DisplayName = "Less Than or Equal"),
	Equal				UMETA(DisplayName = "Equal"),
	GreaterThanOrEqual	UMETA(DisplayName = "Greater Than or Equal"),
	GreaterThan			UMETA(DisplayName = "Greater Than")
};

/**
 * 
 */
UCLASS()
class COMBAT_API UBTDecorator_CheckCurrentHealth : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckCurrentHealth();

protected:
	UPROPERTY(EditAnywhere)
	EHealthCompareMode CompareMode;

	UPROPERTY(EditAnywhere)
	FGameplayAttribute MaxHealthAttribute;

	UPROPERTY(EditAnywhere)
	FGameplayAttribute CurrentHealthAttribute;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HealthThreshold = 0.5f;

protected:
	//~ Begin UBTDecorator Interface.
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTDecorator Interface
};
