#include "AGStructTypes.h"

#include "AbilitySystem/Abilities/AGHeroGameplayAbility.h"

bool FAGHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
