// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateMotionWarp.generated.h"


class ACombatEnemyCharacter;

/**
 * 
 */
UCLASS()
class COMBAT_API UBTService_UpdateMotionWarp : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateMotionWarp();

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector InTargetActorKey;

private:
	TWeakObjectPtr<ACombatEnemyCharacter> OwningEnemyCharacter;

protected:
	//~ Begin UBTNode Interface.
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	//~ Begin UBTService Interface.
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//~ End UBTService Interface
};
