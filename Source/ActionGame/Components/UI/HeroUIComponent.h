// replcae later

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "GameplayTagContainer.h"
#include "HeroUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdatedDelegate, FGameplayTag, AbilityInputTag, TSoftObjectPtr<UMaterialInterface>, SoftAbilityIconMaterial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldownBeginDelegate, FGameplayTag, AbiltyInputTag, float, TotalCooldownTime, float, RemainingCooldownTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoneInteractedDelegate, bool, bShouldDisplayIcon);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRageChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityIconSlotUpdatedDelegate OnAbilityIconSlotUpdated;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityCooldownBeginDelegate OnAbilityCooldownBegin;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStoneInteractedDelegate OnStoneInteracted;
};
