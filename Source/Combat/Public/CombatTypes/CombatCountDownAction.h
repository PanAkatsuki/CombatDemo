// Zhang All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CombatTypes/CombatEnumTypes.h"

/**
 * 
 */
class COMBAT_API FCombatCountDownAction : public FPendingLatentAction
{
public:
	FCombatCountDownAction() = default;
	FCombatCountDownAction(float InTotalCountDownTime, float InUpdateInterval, float& InOutRemainingTime, ECombatCountDownActionOutput& InCountDownOutput, const FLatentActionInfo& LatentInfo)
		: bNeedToCancel(false),
		TotalCountDownTime(InTotalCountDownTime),
		UpdateInterval(InUpdateInterval),
		OutRemainingTime(InOutRemainingTime),
		CountDownOutput(InCountDownOutput),
		ExecutionFunction(LatentInfo.ExecutionFunction),
		OutputLink(LatentInfo.Linkage),
		CallbackTarget(LatentInfo.CallbackTarget),
		ElapsedInterval(0.f),
		ElspsedTimeSinceStart(0.f)
	{

	}

public:
	virtual void UpdateOperation(FLatentResponse& Response) override;

private:
	bool bNeedToCancel;
	float TotalCountDownTime;
	float UpdateInterval;
	float& OutRemainingTime;
	ECombatCountDownActionOutput& CountDownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElspsedTimeSinceStart;

public:
	void CancelAction();
};
