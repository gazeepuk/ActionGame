// replcae later


#include "AbilitySystem/AbilityTasks/AGAbilityTask_ExecuteTaskOnTick.h"

UAGAbilityTask_ExecuteTaskOnTick::UAGAbilityTask_ExecuteTaskOnTick()
{
	bTickingTask = true;
}

UAGAbilityTask_ExecuteTaskOnTick* UAGAbilityTask_ExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	UAGAbilityTask_ExecuteTaskOnTick* Node = NewAbilityTask<UAGAbilityTask_ExecuteTaskOnTick>(OwningAbility);
	return Node;
}

void UAGAbilityTask_ExecuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}
