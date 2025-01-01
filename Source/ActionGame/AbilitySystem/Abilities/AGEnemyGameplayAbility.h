// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGGameplayAbility.h"
#include "AGEnemyGameplayAbility.generated.h"

class UEnemyCombatComponent;
class AAGEnemyCharacter;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGEnemyGameplayAbility : public UAGGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "ActionGame|Ability")
	AAGEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "ActionGame|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<AAGEnemyCharacter> CachedEnemyCharacter;
};
