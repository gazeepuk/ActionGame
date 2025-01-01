// replcae later


#include "DataAsset_EnemyStartUpData.h"

#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AGEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UAGAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if(EnemyCombatAbilities.IsEmpty())
	{
		return;
	}
	
	for (const TSubclassOf<UAGEnemyGameplayAbility>& EnemyAbilityClass : EnemyCombatAbilities)
	{
		if(!EnemyAbilityClass)
		{
			return;
		}

		FGameplayAbilitySpec AbilitySpec(EnemyAbilityClass);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		InASCToGive->GiveAbility(AbilitySpec);
	} 
}
