// Zhang All Rights Reserved.


#include "AI/BTService_UpdateMotionWarp.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CombatEnemyCharacter.h"
#include "MotionWarpingComponent.h"
#include "AIController.h"

#include "CombatDebugHelper.h"


UBTService_UpdateMotionWarp::UBTService_UpdateMotionWarp()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	Interval = 0.2f;
	RandomDeviation = 0.f;

	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTService_UpdateMotionWarp::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BlackboardData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("In UBTService_GetDistToTarget::InitializeFromAsset, BlackboardData is null!"));
	}
}

FString UBTService_UpdateMotionWarp::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Rotate to %s"), *KeyDescription);
}

void UBTService_UpdateMotionWarp::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwningEnemyCharacter.IsValid())
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		check(AIController);
		ACombatEnemyCharacter* EnemyCharacter = Cast<ACombatEnemyCharacter>(AIController->GetPawn());
		check(EnemyCharacter);
		OwningEnemyCharacter = EnemyCharacter;
	}

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	check(TargetObject);

	if (AActor* TargetActor = Cast<AActor>(TargetObject))
	{
		OwningEnemyCharacter.Get()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			FName(TEXT("AttackTarget")),
			TargetActor->GetActorLocation()
		);
	}
	else
	{
		Debug::Print(TEXT("In UBTService_UpdateMotionWarp::TickNode, TargetActor cast failed!"));
	}
}
