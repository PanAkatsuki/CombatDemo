// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_OrientToTargetActor();

public:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere)
	float RotaionInterpSpeed = 5.f;

public:
	//~ Begin UBTNode Interface.
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	//~ Begin UBTService Interface.
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//~ End UBTService Interface

};
