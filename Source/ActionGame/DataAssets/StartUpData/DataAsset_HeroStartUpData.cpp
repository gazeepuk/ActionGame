// replcae later


#include "DataAsset_HeroStartUpData.h"

#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"

void UDataAsset_HeroStartUpData::GiveToAbilitySystemComponent(UAGAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	for (const FAGHeroAbilitySet& AbilitySet : HeroStartUpAbilitySet)
	{
		if(!AbilitySet.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
