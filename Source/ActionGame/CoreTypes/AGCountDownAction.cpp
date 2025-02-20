// replcae later


#include "CoreTypes/AGCountDownAction.h"

void FAGCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}

void FAGCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		CountDownOutput = EAGCountDownActionOutput::Canceled;

		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);

		return;
	}

	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = EAGCountDownActionOutput::Completed;

		Response.FinishAndTriggerIf(true,ExecutionFunction,OutputLink,CallbackTarget);

		return;
	}

	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();

		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

		CountDownOutput = EAGCountDownActionOutput::Updated;

		Response.TriggerLink(ExecutionFunction,OutputLink,CallbackTarget);
		  
		ElapsedInterval = 0.f;
	}
}
