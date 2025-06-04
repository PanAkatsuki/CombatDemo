// Zhang All Rights Reserved.


#include "AI/BTTask_RotateToFaceTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

#include "CombatDebugHelper.h"


UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	InTargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetToFaceKey), AActor::StaticClass());
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetToFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceTargetTaskMemory);
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetToFaceKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Rotates to %s Key until the angle precision %s is reached"), *KeyDescription, *FString::SanitizeFloat(AnglePrecision));
}

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Super::ExecuteTask(OwnerComp, NodeMemory);// Should NOT Call Super::ExecuteTask(OwnerComp, NodeMemory), cus in this Function, there is a return function

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetToFaceKey.SelectedKeyName);
	if (!ActorObject)
	{
		//Debug::Print(TEXT("!ActorObject"));
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(ActorObject);
	check(TargetActor);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	check(Memory);

	Memory->OwningPawn = OwningPawn;
	Memory->TargetActor = TargetActor;

	if (!Memory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

	if (HasReachedAnglePercision(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
	{
		Memory->Reset();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds); // Super::TickTask function is empty

	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);

	if (!Memory->IsValid())
	{
		Debug::Print(TEXT("In UBTTask_RotateToFaceTarget::TickTask, Memory is not valid!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (HasReachedAnglePercision(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
	{
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetToFaceKey.SelectedKeyName);
		AActor* TargetActor = Cast<AActor>(ActorObject);

		APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

		if (TargetActor && OwningPawn)
		{
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
			const FRotator TargetRotation = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRotation, DeltaSeconds, RotationInterpspeed);
			OwningPawn->SetActorRotation(TargetRotation);
		}
	}
}

bool UBTTask_RotateToFaceTarget::HasReachedAnglePercision(APawn* QueryPawn, AActor* TargetActor) const
{
	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	const FVector OwnerToTargetNormalized = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalized);
	const float AngleDiff = UKismetMathLibrary::DegAcos(DotResult);

	return AngleDiff <= AnglePrecision;
}
