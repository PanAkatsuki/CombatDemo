// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ShouldAbortAllLogic.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTDecorator_ShouldAbortAllLogic : public UBTDecorator
{
	GENERATED_BODY()

protected:
	UBTDecorator_ShouldAbortAllLogic();
	
protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere)
	FGameplayTag TagToCheck;

protected:
	//~ Begin UBTDecorator Interface.
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTDecorator Interface
};
