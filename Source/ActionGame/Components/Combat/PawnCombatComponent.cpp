// replcae later


#include "PawnCombatComponent.h"

#include "Components/BoxComponent.h"
#include "Items/Weapons/AGWeaponBase.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AAGWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("%s has been already added as carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);
	
	if(bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}
}

AAGWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if(CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		if(AAGWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

AAGWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if(!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if(ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AAGWeaponBase* WeaponToToggle =  GetCharacterCurrentEquippedWeapon();
		check(WeaponToToggle);

		if(bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
			OverlappedActors.Empty();
		}
	}

	//TODO: Handle collision boxes
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractingActor)
{
}
