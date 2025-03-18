// replcae later


#include "EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/AGEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "CoreTypes/AGGameplayTags.h"
#include "FunctionLibraries/AGFunctionLibrary.h"


void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if(OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	// Validate player's block 
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UAGFunctionLibrary::NativeDoesActorHaveTag(HitActor, AGGameplayTags::Player_Status_Blocking);
	const bool bIsAttackUnblockable = UAGFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn(),AGGameplayTags::Enemy_Status_Unblockable);
	
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
