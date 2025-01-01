// replcae later


#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"

void UAGAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if(!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			continue;
		}

		TryActivateAbility(AbilitySpec.Handle);
	} 
}

void UAGAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}

void UAGAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FAGHeroAbilitySet>& InDefaultWeaponAbilities,
	int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitiesSpecHandles)
{
	if(InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	for (const FAGHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if(!AbilitySet.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		
		FGameplayAbilitySpecHandle OutAbilitySpecHandle = GiveAbility(AbilitySpec);

		OutGrantedAbilitiesSpecHandles.AddUnique(OutAbilitySpecHandle);
	}
}

void UAGAbilitySystemComponent::RemoveGrantedHeroWeaponAbilities(
	TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if(InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if(SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	InSpecHandlesToRemove.Empty();
}
