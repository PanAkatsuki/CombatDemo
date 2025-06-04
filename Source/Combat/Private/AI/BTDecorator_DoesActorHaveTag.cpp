// Zhang All Rights Reserved.


#include "AI/BTDecorator_DoesActorHaveTag.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "CombatFunctionLibrary.h"

#include "CombatDebugHelper.h"


UBTDecorator_DoesActorHaveTag::UBTDecorator_DoesActorHaveTag()
{
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

bool UBTDecorator_DoesActorHaveTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	check(TargetObject);
	AActor* TargetActor = Cast<AActor>(TargetObject);
	check(TargetActor);

	if (UCombatFunctionLibrary::NativeDoesActorHaveTag(TargetActor, TagToCheck))
	{
		if (bInverseConditionCheck)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		if (bInverseConditionCheck)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

FString UBTDecorator_DoesActorHaveTag::GetStaticDescription() const
{
	const FString TagName = TagToCheck.ToString();

	return FString::Printf(TEXT("Check if actor has %s tag"), *TagName);
}
