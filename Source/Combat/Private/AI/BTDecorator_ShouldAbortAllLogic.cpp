// Zhang All Rights Reserved.


#include "AI/BTDecorator_ShouldAbortAllLogic.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "CombatFunctionLibrary.h"
#include "AIController.h"

#include "CombatDebugHelper.h"


UBTDecorator_ShouldAbortAllLogic::UBTDecorator_ShouldAbortAllLogic()
{
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());

	bNotifyTick = true;
}

void UBTDecorator_ShouldAbortAllLogic::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!CachedActor.IsValid())
	{
		UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
		if (!TargetObject)
		{
			return;
		}
		CachedActor = Cast<AActor>(TargetObject);
	}
	check(CachedActor.IsValid());

	AActor* TargetActor = CachedActor.Get();
	check(TargetActor);

	APawn* OwnerPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	check(OwnerPawn);

	const bool bTargetDead = UCombatFunctionLibrary::NativeDoesActorHaveTag(TargetActor, TagToCheck);
	const bool bOwnerDead = UCombatFunctionLibrary::NativeDoesActorHaveTag(OwnerPawn, TagToCheck);

	if (bTargetDead || bOwnerDead)
	{
		OwnerComp.RequestExecution(this);
	}
}

bool UBTDecorator_ShouldAbortAllLogic::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	if (!TargetObject)
	{
		return true;
	}

	AActor* TargetActor = Cast<AActor>(TargetObject);
	check(TargetActor);

	APawn* OwnerPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	check(OwnerPawn);

	const bool bTargetDead = UCombatFunctionLibrary::NativeDoesActorHaveTag(TargetActor, TagToCheck);
	const bool bOwnerDead = UCombatFunctionLibrary::NativeDoesActorHaveTag(OwnerPawn, TagToCheck);

	return bTargetDead || bOwnerDead;
}

FString UBTDecorator_ShouldAbortAllLogic::GetStaticDescription() const
{
	return FString::Printf(TEXT("Stop all logic"));
}
