#pragma once
#include "GameplayTagContainer.h"

#include "AGStructTypes.generated.h"

class UAGHeroGameplayAbility;
class UInputMappingContext;
class UAGHeroLinkedAnimLayer;

USTRUCT(BlueprintType)
struct FAGHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTags"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAGHeroGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAGHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAGHeroLinkedAnimLayer> WeaponAnimLayerToLink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> WeaponInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FAGHeroAbilitySet> DefaultWeaponAbilities;
};
