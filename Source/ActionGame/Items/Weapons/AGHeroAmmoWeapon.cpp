// replcae later


#include "AGHeroAmmoWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CoreTypes/AGGameplayTags.h"
#include "FunctionLibraries/AGFunctionLibrary.h"


AAGHeroAmmoWeapon::AAGHeroAmmoWeapon()
{
	CurrentAmmo = MaxAmmo;
}

void AAGHeroAmmoWeapon::SetCurrentAmmo(int32 InCurrentAmmo)
{
	CurrentAmmo = FMath::Clamp(InCurrentAmmo, 0, MaxAmmo);

	OnWeaponAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo, bInfiniteAmmo);
}

bool AAGHeroAmmoWeapon::HasEnoughAmmo(const int32 InAmmoToSpend) const
{
	return bInfiniteAmmo || CurrentAmmo >= InAmmoToSpend;
}

bool AAGHeroAmmoWeapon::ShouldReload() const
{
	return bInfiniteAmmo || CurrentAmmo > 0;
}

void AAGHeroAmmoWeapon::ReloadAmmo()
{
	SetCurrentAmmo(MaxAmmo);
}

bool AAGHeroAmmoWeapon::TryShoot(int32 InAmmoToSpend)
{
	if(HasEnoughAmmo(InAmmoToSpend))
	{
		if(!bInfiniteAmmo)
		{
			SetCurrentAmmo(CurrentAmmo - InAmmoToSpend);
		}

		return true;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), AGGameplayTags::Player_Event_Ammo_Reload, FGameplayEventData());

	return false;
}

void AAGHeroAmmoWeapon::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentAmmo(MaxAmmo);
}
