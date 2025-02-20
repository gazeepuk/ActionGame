// replcae later

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CoreTypes/AGEnumTypes.h"
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

	UFUNCTION(BlueprintPure, Category = "ActionGame|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "ActionGame|Ability")
	UAGAbilitySystemComponent* GetAGAbilitySystemComponentFromAvatarActor() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EAGSuccessType& OutSuccessType);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|Ability")
	bool GetRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime);

	UFUNCTION(BlueprintCallable, Category = "ActionGame|Ability")
	void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& HitResults, const bool bTargetHostile = true);
};
