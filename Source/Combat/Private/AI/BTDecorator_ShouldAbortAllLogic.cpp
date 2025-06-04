// Zhang All Rights Reserved.


#include "AI/BTDecorator_ShouldAbortAllLogic.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "CombatFunctionLibrary.h"
#include "AIController.h"

#include "CombatDebugHelper.h"


UBTDecorator_ShouldAbortAllLogic::UBTDecorator_ShouldAbortAllLogic()
{
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

bool UBTDecorator_ShouldAbortAllLogic::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	if (!TargetObject)
	{
		return false;
	}

	AActor* TargetActor = Cast<AActor>(TargetObject);
	check(TargetActor);

	bool bIsTargetDead = false;
	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(TargetActor, TagToCheck))
	{
		bIsTargetDead = true;
	}

	bool bIsOwnerDead = false;
	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(OwnerComp.GetAIOwner()->GetPawn(), TagToCheck))
	{
		bIsOwnerDead = true;
	}

	if (bIsTargetDead || bIsOwnerDead)
	{
		return true;
	}

	return false;
}

FString UBTDecorator_ShouldAbortAllLogic::GetStaticDescription() const
{
	return FString::Printf(TEXT("Stop all logic"));
}
