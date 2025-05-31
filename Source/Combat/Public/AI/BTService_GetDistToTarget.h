// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetDistToTarget.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTService_GetDistToTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_GetDistToTarget();

protected:
	UPROPERTY(EditDefaultsOnly)
	FBlackboardKeySelector InTargetActorKey;

protected:
	//~ Begin UBTService Interface.
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//~ End UBTService Interface

};
