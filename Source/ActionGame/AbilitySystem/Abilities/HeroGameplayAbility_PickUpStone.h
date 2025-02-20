// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGHeroGameplayAbility.h"
#include "HeroGameplayAbility_PickUpStone.generated.h"

class AStonePickUpBase;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UHeroGameplayAbility_PickUpStone : public UAGHeroGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	void CollectStone();

	UFUNCTION(BlueprintCallable)
	void ConsumeStones();
	
private:
	UPROPERTY(EditDefaultsOnly)
	float TraceDistance;
	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(100.f);
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> StoneTraceChannel;
	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebug;
	UPROPERTY()
	TArray<AStonePickUpBase*> CollectedStones;
};
