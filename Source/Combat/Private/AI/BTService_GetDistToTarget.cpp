// Zhang All Rights Reserved.


#include "AI/BTService_GetDistToTarget.h"

#include "BehaviorTree/BTFunctionLibrary.h"


UBTService_GetDistToTarget::UBTService_GetDistToTarget()
{
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTService_GetDistToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//UBTFunctionLibrary::GetBlackboardValueAsActor()
}
