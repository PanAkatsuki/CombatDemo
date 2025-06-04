// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_DoesActorHaveTag.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTDecorator_DoesActorHaveTag : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_DoesActorHaveTag();

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere)
	FGameplayTag TagToCheck;

	UPROPERTY(EditAnywhere)
	bool bInverseConditionCheck = false;

protected:
	//~ Begin UBTDecorator Interface.
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTDecorator Interface
};
