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

void UEnemyCombatComponent::ToggleHandsCollisions(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	AAGEnemyCharacter* OwningEnemy =  GetOwner<AAGEnemyCharacter>();
	check(OwningEnemy);

	UBoxComponent* LeftHandCollisionBox = OwningEnemy->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningEnemy->GetRightHandCollisionBox();
	check(LeftHandCollisionBox && RightHandCollisionBox);
	
	switch (ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		
		if(bShouldEnable)
		{
 			LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		}
		else
		{
			LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		}
		
		break;
		
	case EToggleDamageType::RightHand:
		
		if(bShouldEnable)
		{
			RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		}
		else
		{
			RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		}
		break;
		
	default:
		break;
	}
	
	if(!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}
