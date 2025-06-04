// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ComputeSuccessChange.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTDecorator_ComputeSuccessChange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_ComputeSuccessChange();

protected:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SuccessChance = 0.5f;

protected:
	//~ Begin UBTDecorator Interface.
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTDecorator Interface

};
