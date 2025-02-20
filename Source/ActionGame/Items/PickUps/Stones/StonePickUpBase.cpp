// replcae later


#include "StonePickUpBase.h"

#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "Characters/AGHeroCharacter.h"
#include "CoreTypes/AGGameplayTags.h"


void AStonePickUpBase::Consume(UAGAbilitySystemComponent* InASC, int32 InApplyLevel)
{
	check(StoneGameplayEffectClass);

	UGameplayEffect* StoneGameplayEffect = StoneGameplayEffectClass.GetDefaultObject();
	
	InASC->ApplyGameplayEffectToSelf(StoneGameplayEffect, InApplyLevel, InASC->MakeEffectContext());

	BP_OnStoneConsumed();
}

void AStonePickUpBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AAGHeroCharacter* OverlappedHeroCharacter = Cast<AAGHeroCharacter>(OtherActor))
	{
		OverlappedHeroCharacter->GetAGAbilitySystemComponent()->TryActivateAbilityByTag(AGGameplayTags::Player_Ability_PickUp_Stone);
	}
}

void AStonePickUpBase::OnPickUpCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnPickUpCollisionSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
