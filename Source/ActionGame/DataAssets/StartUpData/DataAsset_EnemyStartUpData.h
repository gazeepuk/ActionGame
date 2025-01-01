// replcae later

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_StartUpDataBase.h"
#include "DataAsset_EnemyStartUpData.generated.h"

class UAGEnemyGameplayAbility;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UDataAsset_EnemyStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UAGAbilitySystemComponent* InASCToGive, int32 ApplyLevel) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UAGEnemyGameplayAbility>> EnemyCombatAbilities;
};
