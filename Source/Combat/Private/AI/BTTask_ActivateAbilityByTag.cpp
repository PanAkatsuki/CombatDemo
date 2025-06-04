// Zhang All Rights Reserved.


#include "AI/BTTask_ActivateAbilityByTag.h"

#include "AIController.h"
#include "Characters/CombatEnemyCharacter.h"

#include "AbilitySystem/CombatAbilitySystemComponent.h"

#include "CombatDebugHelper.h"


UBTTask_ActivateAbilityByTag::UBTTask_ActivateAbilityByTag()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

void UBTTask_ActivateAbilityByTag::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

}

uint16 UBTTask_ActivateAbilityByTag::GetInstanceMemorySize() const
{
	return sizeof(FActivateAbilityByTagTaskMemory);
}

FString UBTTask_ActivateAbilityByTag::GetStaticDescription() const
{
	const FString KeyDescription = AbilityTagToActivate.ToString();

	return FString::Printf(TEXT("Activate %s"), *KeyDescription);
}

EBTNodeResult::Type UBTTask_ActivateAbilityByTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Should not call super, for there is a return function

	FActivateAbilityByTagTaskMemory* Memory = CastInstanceNodeMemory<FActivateAbilityByTagTaskMemory>(NodeMemory);
	check(Memory);

	if (Memory->OwningCharacter.IsValid())
	{
		OnExecuteTask(OwnerComp, Memory);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		if (AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner()))
		{
			if (ACombatEnemyCharacter* Enemy = Cast<ACombatEnemyCharacter>(AIController->GetPawn()))
			{
				Memory->OwningCharacter = Enemy;
				OnExecuteTask(OwnerComp, Memory);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
			else
			{
				Debug::Print(TEXT("In EBTNodeResult::Type UBTTask_ToggleStrafingState::ExecuteTask, Enemy cast fail!"));
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return EBTNodeResult::Failed;
			}
		}
		else
		{
			Debug::Print(TEXT("In EBTNodeResult::Type UBTTask_ToggleStrafingState::ExecuteTask, AIController cast fail!"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}
	}
}

void UBTTask_ActivateAbilityByTag::OnExecuteTask(UBehaviorTreeComponent& OwnerComp, FActivateAbilityByTagTaskMemory* Memory)
{
	if (ACombatEnemyCharacter* EnemyCharacter = Memory->OwningCharacter.Get())
	{
		EnemyCharacter->GetCombatAbilitySystemComponent()->TryAcitivateAbilityByTag(AbilityTagToActivate);
	}
}
