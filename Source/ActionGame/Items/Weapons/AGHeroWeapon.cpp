// replcae later


#include "AGHeroWeapon.h"

void AAGHeroWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitiesSpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AAGHeroWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitiesSpecHandles;
}
