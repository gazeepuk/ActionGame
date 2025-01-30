// replcae later


#include "EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CoreTypes/AGGameplayTags.h"


void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if(OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	// TODO:: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = false;
	const bool bIsAttackUnblockable = false;

	if(bIsPlayerBlocking && !bIsAttackUnblockable)
	{
		// TODO:: check if block is valid
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;
	
	if(bIsValidBlock)
	{
		// TODO:: Handle successful block
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), AGGameplayTags::Shared_Event_MeleeHit, EventData);
	}
}
