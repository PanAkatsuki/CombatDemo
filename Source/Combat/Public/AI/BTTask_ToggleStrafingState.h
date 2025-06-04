// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ToggleStrafingState.generated.h"


class ACombatEnemyCharacter;

struct FToggleStrafingStateTaskMemory
{
	TWeakObjectPtr<ACombatEnemyCharacter> OwningCharacter;
};

/**
 * 
 */
UCLASS()
class COMBAT_API UBTTask_ToggleStrafingState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ToggleStrafingState();

protected:
	UPROPERTY(EditAnywhere)
	bool bShouldEnable = false;

	UPROPERTY(EditAnywhere)
	bool bShouldChangeMaxWalkSpeed = false;

	UPROPERTY(EditAnywhere)
	float StrafingWalkSpeed = 200.f;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector InDefaultMaxWalkSpeedKey;

	UPROPERTY(EditAnywhere)
	FGameplayTag StrafingStatusTag;

protected:
	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	//~ Begin UBTTaskNode Interface
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//~ End UBTTaskNode Interface

	virtual void OnExecuteTask(UBehaviorTreeComponent& OwnerComp, FToggleStrafingStateTaskMemory* Memory);
};
