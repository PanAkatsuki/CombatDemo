// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

struct FRotateToFaceTargetTaskMemory
{
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;

	bool IsValid()
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}

	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}
};

/**
 * 
 */
UCLASS()
class COMBAT_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RotateToFaceTarget();
	
private:
	UPROPERTY(EditAnywhere, Category = "Face Target")
	float AnglePrecision = 10.f;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	float RotationInterpspeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	FBlackboardKeySelector InTargetToFaceKey;

private:
	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	//~ Begin UBTTaskNode Interface
	// If EBTNodeResult::Type ExecuteTask function return EBTNodeResult::InProgress, than system will call TickTask function
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//~ End UBTTaskNode Interface

	bool HasReachedAnglePercision(APawn* QueryPawn, AActor* TargetActor) const;
};
