// Zhang All Rights Reserved.


#include "AI/BTTask_ToggleStrafingState.h"

#include "Characters/CombatEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "CombatDebugHelper.h"


UBTTask_ToggleStrafingState::UBTTask_ToggleStrafingState()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

void UBTTask_ToggleStrafingState::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		InDefaultMaxWalkSpeedKey.ResolveSelectedKey(*BlackboardData);
	}
}

uint16 UBTTask_ToggleStrafingState::GetInstanceMemorySize() const
{
	return sizeof(FToggleStrafingStateTaskMemory);
}

FString UBTTask_ToggleStrafingState::GetStaticDescription() const
{
	if (bShouldEnable)
	{
		return FString::Printf(TEXT("Toggle strafing state to true"));
	}
	else
	{
		return FString::Printf(TEXT("Toggle strafing state to false"));
	}
}

EBTNodeResult::Type UBTTask_ToggleStrafingState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Should not call super, for there is a return function

	FToggleStrafingStateTaskMemory* Memory = CastInstanceNodeMemory<FToggleStrafingStateTaskMemory>(NodeMemory);
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

void UBTTask_ToggleStrafingState::OnExecuteTask(UBehaviorTreeComponent& OwnerComp, FToggleStrafingStateTaskMemory* Memory)
{
	if (bShouldEnable)
	{
		Memory->OwningCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		if (bShouldChangeMaxWalkSpeed)
		{
			Memory->OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = StrafingWalkSpeed;
		}

		UCombatFunctionLibrary::AddGameplayTagToActorIfNone(Memory->OwningCharacter.Get(), StrafingStatusTag);
	}
	else
	{
		Memory->OwningCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

		float DefaultMaxWalkSpeed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(InDefaultMaxWalkSpeedKey.SelectedKeyName);
		if (Memory->OwningCharacter->GetCharacterMovement()->MaxWalkSpeed != DefaultMaxWalkSpeed)
		{
			Memory->OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
		}

		UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(Memory->OwningCharacter.Get(), StrafingStatusTag);
	}
}
