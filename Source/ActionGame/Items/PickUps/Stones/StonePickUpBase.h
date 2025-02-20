// replcae later

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/AGPickUpBase.h"
#include "StonePickUpBase.generated.h"

class UGameplayEffect;
class UAGAbilitySystemComponent;

UCLASS()
class ACTIONGAME_API AStonePickUpBase : public AAGPickUpBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Consume(UAGAbilitySystemComponent* InASC, int32 InApplyLevel);
	
protected:

	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnPickUpCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;
};
