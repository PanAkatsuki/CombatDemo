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
	//~ Begin UBTNode Interface.
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	//~ End UBTNode Interface

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Target")
	float RotaionInterpSpeed = 5.f;

public:
	//~ Begin UBTNode Interface.
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

};
