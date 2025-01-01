// replcae later

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UGameplayEffect;
class UAGAbilitySystemComponent;
class UAGGameplayAbility;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UAGAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UAGGameplayAbility>> ActivateOnGivenAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UAGGameplayAbility>> ReactiveAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;
	
	void GrantAbilities(const TArray<TSubclassOf<UAGGameplayAbility>>& InAbilitiesToGive, UAGAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
