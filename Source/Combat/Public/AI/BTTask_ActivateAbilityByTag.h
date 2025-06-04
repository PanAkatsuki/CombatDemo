// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateAbilityByTag.generated.h"


class ACombatEnemyCharacter;

struct FActivateAbilityByTagTaskMemory
{
	TWeakObjectPtr<ACombatEnemyCharacter> OwningCharacter;
};

/**
 * 
 */
UCLASS()
class COMBAT_API UBTTask_ActivateAbilityByTag : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ActivateAbilityByTag();

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag AbilityTagToActivate;

protected:
	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	//~ Begin UBTTaskNode Interface
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//~ End UBTTaskNode Interface

	virtual void OnExecuteTask(UBehaviorTreeComponent& OwnerComp, FActivateAbilityByTagTaskMemory* Memory);
};
