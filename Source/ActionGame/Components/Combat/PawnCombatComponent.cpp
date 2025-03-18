// replcae later


#include "PawnCombatComponent.h"

#include "Characters/AGBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/AGWeaponBase.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AAGWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("%s has been already added as carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	// Add Registered Weapon to TMap
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	// Subscribe to Registered Weapon's delagates
	InWeaponToRegister->OnWeaponHitTarget.AddDynamic(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.AddDynamic(this, &ThisClass::OnWeaponPulledFromTargetActor);
	
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

void UPawnCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
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

void UPawnCombatComponent::ToggleHandsCollisions(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	AAGBaseCharacter* OwningCharacter =  GetOwner<AAGBaseCharacter>();
	check(OwningCharacter);

	UBoxComponent* LeftHandCollisionBox = OwningCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningCharacter->GetRightHandCollisionBox();
	check(LeftHandCollisionBox && RightHandCollisionBox);

	// Toggle hands' collisions 
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

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	switch(ToggleDamageType)
	{
	case EToggleDamageType::CurrentEquippedWeapon:
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
		break;
		
	case EToggleDamageType::LeftHand:
	case EToggleDamageType::RightHand:
		ToggleHandsCollisions(bShouldEnable, ToggleDamageType);
		break;
		
	default:
		break;
	}
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractingActor)
{
}
