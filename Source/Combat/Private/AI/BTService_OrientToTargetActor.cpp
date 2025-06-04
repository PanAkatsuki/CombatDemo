// Zhang All Rights Reserved.


#include "AI/BTService_OrientToTargetActor.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

#include "CombatDebugHelper.h"


UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	Interval = 0.f;
	RandomDeviation = 0.f;

	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BlackboardData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("In UBTService_OrientToTargetActor::InitializeFromAsset, BlackboardData is null!"));
	}
}

FString UBTService_OrientToTargetActor::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Orient rotation to %s key %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UBTService_OrientToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (TargetActor && OwningPawn)
	{
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		const FRotator TargetRotation = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRotation, DeltaSeconds, RotaionInterpSpeed);

		OwningPawn->SetActorRotation(TargetRotation);
	}
}
