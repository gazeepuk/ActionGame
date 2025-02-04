// replcae later


#include "EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CoreTypes/AGGameplayTags.h"
#include "FunctionLibraries/AGFunctionLibrary.h"


void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if(OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	// TODO:: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UAGFunctionLibrary::NativeDoesActorHaveTag(HitActor, AGGameplayTags::Player_Status_Blocking);
	const bool bIsAttackUnblockable = false;

	if(bIsPlayerBlocking && !bIsAttackUnblockable)
	{
		bIsValidBlock = UAGFunctionLibrary::IsValidBlock(GetOwner(), HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;
	
	if(bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, AGGameplayTags::Player_Event_SuccessfulBlock, EventData);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), AGGameplayTags::Shared_Event_MeleeHit, EventData);
	}
}
