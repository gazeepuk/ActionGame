// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UInputMappingContext;
class UAGWidgetBase;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UHeroGameplayAbility_TargetLock : public UAGHeroGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);
private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();
	AActor* GetNearestActorFromAvailableActors(const TArray<AActor*>& InAvailableActors) const;
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);
	void DrawTargetLockWidget();
	void SetTargetLockWidgetPosition();
	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);
	
	void InitTargetLockMovement();
	void InitTargetLockMappingContext();
	
	void CancelTargetLockAbility();
	void CleanUp();
	void ResetTargetLockMovement();
	void ResetTargetLockMappingContext();
	
	UPROPERTY(EditDefaultsOnly, Category = "TargetLock", meta = (UIMin = "0"))
	float TraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	bool bShowPersistentDebugShape = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TSubclassOf<UAGWidgetBase> TargetLockWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock", meta = (UIMin = "0"))
	float TargetLockRotationInterpSpeed = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "TargetLock", meta = (UIMin = "0"))
	float TargetLockMaxWalkSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TObjectPtr<UInputMappingContext> TargetLockMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float TargetLockCameraOffsetDistance = 20.f;
	
	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	UPROPERTY()
	AActor* CurrentLockedActor;

	TWeakObjectPtr<UAGWidgetBase> TargetLockWidget;
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	float CachedDefaultMaxWalkSpeed = 0.f;
};
