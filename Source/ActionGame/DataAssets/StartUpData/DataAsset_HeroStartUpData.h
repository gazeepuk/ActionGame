// replcae later

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_StartUpDataBase.h"
#include "GameplayTagContainer.h"
#include "CoreTypes/AGStructTypes.h"
#include "DataAsset_HeroStartUpData.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UDataAsset_HeroStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UAGAbilitySystemComponent* InASCToGive, int32 ApplyLevel) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FAGHeroAbilitySet> HeroStartUpAbilitySet;
};
