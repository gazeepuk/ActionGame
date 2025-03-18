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

	// Find Matching Abilities by Input Tag and activate them
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!AbilitySpec.DynamicAbilityTags.HasTag(InInputTag))
		{
			continue;
		}

		// Toggle Abilities
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

	// Find Matching Abilities by Input Tag and cancel them
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	} 
}

void UAGAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FAGHeroAbilitySet>& InDefaultWeaponAbilities,
                                                         const TArray<FAGHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitiesSpecHandles)
{
	if(InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	// Grant Default Weapon Abilities 
	for (const FAGHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if(!AbilitySet.IsValid())
		{
			continue;
		}

		// Set up ability's properties
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		// Give Ability
		FGameplayAbilitySpecHandle OutAbilitySpecHandle = GiveAbility(AbilitySpec);

		OutGrantedAbilitiesSpecHandles.AddUnique(OutAbilitySpecHandle);
	}

	// Grant Special Weapon Abilities 
	for (const FAGHeroSpecialAbilitySet& AbilitySet : InSpecialWeaponAbilities)
	{
		if(!AbilitySet.IsValid())
		{
			continue;
		}

		// Set up ability's properties
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		// Give Ability
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

	// Remove all passed Abilities
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
	// Find Ability by Tag and try to activate it
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
