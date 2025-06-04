// Zhang All Rights Reserved.


#include "AI/BTDecorator_ComputeSuccessChange.h"

#include "Kismet/KismetMathLibrary.h"

#include "CombatDebugHelper.h"


UBTDecorator_ComputeSuccessChange::UBTDecorator_ComputeSuccessChange()
{
}

bool UBTDecorator_ComputeSuccessChange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return UKismetMathLibrary::RandomBoolWithWeight(SuccessChance);
}

FString UBTDecorator_ComputeSuccessChange::GetStaticDescription() const
{
	return FString::Printf(TEXT("Enter with %.2f chance"), SuccessChance);
}
