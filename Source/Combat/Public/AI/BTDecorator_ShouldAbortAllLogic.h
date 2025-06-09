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

private:
	TWeakObjectPtr<AActor> CachedActor;

protected:
	//~ Begin UBTDecorator Interface.
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTDecorator Interface
};
