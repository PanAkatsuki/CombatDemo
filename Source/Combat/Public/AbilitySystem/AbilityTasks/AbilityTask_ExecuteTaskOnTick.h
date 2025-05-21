// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ExecuteTaskOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class COMBAT_API UAbilityTask_ExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAbilityTask_ExecuteTaskOnTick();

public:
	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick;
public:
	UFUNCTION(BlueprintCallable, Category = "Combat | AbilityTasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_ExecuteTaskOnTick* ExecuteTasksOnTick(UGameplayAbility* OwningAbility);

	//~ Begin UGameplayTask Interface ~//
	virtual void TickTask(float DeltaTime) override;
	//~ End UGameplayTask Interface ~//
};
