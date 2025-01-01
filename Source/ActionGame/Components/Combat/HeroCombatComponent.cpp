// replcae later


#include "HeroCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CoreTypes/AGGameplayTags.h"
#include "Items/Weapons/AGHeroWeapon.h"


AAGHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<AAGHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if(OverlappedActors.Contains(HitActor) || !HitActor)
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), AGGameplayTags::Shared_Event_MeleeHit, Data);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractingActor)
{
}
