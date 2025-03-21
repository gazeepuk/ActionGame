﻿// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGGameplayAbility.h"
#include "AGHeroGameplayAbility.generated.h"

class UHeroUIComponent;
class UHeroCombatComponent;
class AAGHeroPlayerController;
class AAGHeroCharacter;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGHeroGameplayAbility : public UAGGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	AAGHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	AAGHeroPlayerController* GetHeroPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);
	
	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	UHeroUIComponent* GetHeroUIComponentFromActorInfo();
private:
	TWeakObjectPtr<AAGHeroCharacter> CachedHeroCharacter;
	TWeakObjectPtr<AAGHeroPlayerController> CachedHeroPlayerController;
};
