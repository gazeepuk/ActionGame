	// replcae later


#include "DataAsset_StartUpDataBase.h"

#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UAGAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);

	if(!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& GameplayEffect : StartUpGameplayEffects)
		{
			if(!GameplayEffect)
			{
				continue;
			}

			UGameplayEffect* EffectDefaultObject = GameplayEffect.GetDefaultObject();
			InASCToGive->ApplyGameplayEffectToSelf(EffectDefaultObject, ApplyLevel, InASCToGive->MakeEffectContext());
		}
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UAGGameplayAbility>>& InAbilitiesToGive,
	UAGAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if(InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UAGGameplayAbility>& Abilitie : InAbilitiesToGive)
	{
		if(!Abilitie)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(Abilitie);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
