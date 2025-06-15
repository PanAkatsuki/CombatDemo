// Zhang All Rights Reserved.


#include "CombatTypes/CombatCountDownAction.h"

#include "CombatDebugHelper.h"

void FCombatCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		CountDownOutput = ECombatCountDownActionOutput::Cancelled;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}

	if (ElspsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = ECombatCountDownActionOutput::Completed;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}

	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElspsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();

		OutRemainingTime = TotalCountDownTime - ElspsedTimeSinceStart;

		CountDownOutput = ECombatCountDownActionOutput::Updated;

		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);

		ElapsedInterval = 0.f;
	}
}

void FCombatCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
