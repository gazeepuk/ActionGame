// replcae later


#include "AGHeroAmmoWeapon.h"


AAGHeroAmmoWeapon::AAGHeroAmmoWeapon()
{
	CurrentAmmo = MaxAmmo;
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
	CurrentAmmo = MaxAmmo;
}
