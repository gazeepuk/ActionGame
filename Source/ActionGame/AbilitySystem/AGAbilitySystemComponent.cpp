// replcae later


#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AGGameplayAbility.h"
#include "AbilitySystem/Abilities/AGHeroGameplayAbility.h"
#include "CoreTypes/AGGameplayTags.h"

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

		if(InInputTag.MatchesTag(AGGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
		else
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
		
	} 
}

void UAGAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if(!InInputTag.IsValid() || !InInputTag.MatchesTag(AGGameplayTags::InputTag_MustBeHeld))
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	} 
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

bool UAGAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	if(!FoundAbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate =  FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate)

		if(!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}

	return false;
}
