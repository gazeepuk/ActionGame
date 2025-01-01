// replcae later

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AGGameplayAbility.generated.h"


class UAGAbilitySystemComponent;
class UPawnCombatComponent;

UENUM(BlueprintType)
enum class EAGAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:

	//~ Begin UGameplayAbility Interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface
	
	UPROPERTY(EditDefaultsOnly, Category = "AGAbility")
	EAGAbilityActivationPolicy AbilityActivationPolicy{EAGAbilityActivationPolicy::OnTriggered};

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Character|Ability")
	UAGAbilitySystemComponent* GetAGAbilitySystemComponentFromAvatarActor() const;	
};
