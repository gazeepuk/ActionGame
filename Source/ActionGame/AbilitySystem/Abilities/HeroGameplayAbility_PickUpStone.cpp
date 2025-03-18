// replcae later


#include "HeroGameplayAbility_PickUpStone.h"

#include "Components/UI/HeroUIComponent.h"
#include "Items/PickUps/Stones/StonePickUpBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UHeroGameplayAbility_PickUpStone::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                       const FGameplayEventData* TriggerEventData)
{
	GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(true);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_PickUpStone::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	GetHeroUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_PickUpStone::CollectStone()
{
	CollectedStones.Empty();

	// Trace Stones under Avatar Actor
	TArray<FHitResult> HitResults;
	
	const FVector TraceStart = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector TraceEnd = TraceStart - GetAvatarActorFromActorInfo()->GetActorUpVector() * TraceDistance;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetAvatarActorFromActorInfo(),
		TraceStart,
		TraceEnd,
		TraceBoxSize / 2,
		TraceEnd.ToOrientationRotator(),
		StoneTraceChannel,
		false,
		TArray<AActor*>(),
		bDrawDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		HitResults,
		true);

	// Adding every traced stone to CollectedStones array
	for (const FHitResult& HitResult : HitResults)
	{
		if(AStonePickUpBase* StonePickUp = Cast<AStonePickUpBase>(HitResult.GetActor()))
		{
			CollectedStones.AddUnique(StonePickUp);
		}
	}

	// Cancel ability if traced no stones 
	if(CollectedStones.IsEmpty())
	{
		CancelAbility(CurrentSpecHandle, GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UHeroGameplayAbility_PickUpStone::ConsumeStones()
{
	// Cancel ability if there are not any stones to consume 
	if(CollectedStones.IsEmpty())
	{
		CancelAbility(CurrentSpecHandle, GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}

	// Consume all traced stones
	for (AStonePickUpBase* CollectedStone : CollectedStones)
	{
		if(CollectedStone)
		{
			CollectedStone->Consume(GetAGAbilitySystemComponentFromAvatarActor(), GetAbilityLevel());
		}
	}
}
