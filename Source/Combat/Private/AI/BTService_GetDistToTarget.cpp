// Zhang All Rights Reserved.


#include "AI/BTService_GetDistToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CombatDebugHelper.h"


UBTService_GetDistToTarget::UBTService_GetDistToTarget()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	Interval = 0.2f;
	RandomDeviation = 0.f;

	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTService_GetDistToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BlackboardData);
		OutDistToTargetKey.ResolveSelectedKey(*BlackboardData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("In UBTService_GetDistToTarget::InitializeFromAsset, BlackboardData is null!"));
	}
}

FString UBTService_GetDistToTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Distance to %s key is %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UBTService_GetDistToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);

	if (AActor* TargetActor = Cast<AActor>(ActorObject))
	{
		if (AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner()))
		{
			APawn* ControlledPawn = AIController->GetPawn();
			float Dist = ControlledPawn->GetDistanceTo(TargetActor);
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(OutDistToTargetKey.SelectedKeyName, Dist);
		}
		else
		{
			Debug::Print(TEXT("In UBTService_GetDistToTarget::TickNode, AIController is null!"));
		}
	}
	else
	{
		Debug::Print(TEXT("In UBTService_GetDistToTarget::TickNode, TargetActor is null!"));
	}
	
}
