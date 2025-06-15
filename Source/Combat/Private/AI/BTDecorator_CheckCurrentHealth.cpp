// Zhang All Rights Reserved.


#include "AI/BTDecorator_CheckCurrentHealth.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CombatEnemyCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

#include "CombatDebugHelper.h"


UBTDecorator_CheckCurrentHealth::UBTDecorator_CheckCurrentHealth()
{
	bNotifyTick = true;
}

void UBTDecorator_CheckCurrentHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	OwnerComp.RequestExecution(this);
}

bool UBTDecorator_CheckCurrentHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float MaxHealth = 0.f;
	float CurrentHealth = 0.f;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerComp.GetAIOwner()->GetPawn());

	if (ASC)
	{
		bool bIsMaxHealhFound = false;
		MaxHealth = ASC->GetGameplayAttributeValue(MaxHealthAttribute, bIsMaxHealhFound);
		//Debug::Print(TEXT("MaxHealth"), MaxHealth);
		check(bIsMaxHealhFound);

		bool bIsCurrentHealthFound = false;
		CurrentHealth = ASC->GetGameplayAttributeValue(CurrentHealthAttribute, bIsCurrentHealthFound);
		//Debug::Print(TEXT("CurrentHealth"), CurrentHealth);
		check(bIsCurrentHealthFound)
	}
	else
	{
		Debug::Print(TEXT("CastFailed"));
	}

	const float HealthPercent = CurrentHealth / MaxHealth;
	//Debug::Print(TEXT("HealthPercent"), HealthPercent);
	switch (CompareMode)
	{
	case EHealthCompareMode::LessThan:
		if (HealthPercent < HealthThreshold)
		{
			return true;
		}
		break;
	case EHealthCompareMode::LessThanOrEqual:
		if (HealthPercent <= HealthThreshold)
		{
			return true;
		}
		break;
	case EHealthCompareMode::Equal:
		if (HealthPercent == HealthThreshold)
		{
			return true;
		}
		break;
	case EHealthCompareMode::GreaterThanOrEqual:
		if (HealthPercent >= HealthThreshold)
		{
			return true;
		}
		break;
	case EHealthCompareMode::GreaterThan:
		if (HealthPercent > HealthThreshold)
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

FString UBTDecorator_CheckCurrentHealth::GetStaticDescription() const
{
	return FString::Printf(TEXT("Check current health"));
}
