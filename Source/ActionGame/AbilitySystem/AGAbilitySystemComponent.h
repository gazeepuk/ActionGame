// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CoreTypes/AGStructTypes.h"
#include "AGAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|Ability", meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FAGHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitiesSpecHandles);
	
	UFUNCTION(BlueprintCallable, Category = "ActionGame|Ability")
	void RemoveGrantedHeroWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
