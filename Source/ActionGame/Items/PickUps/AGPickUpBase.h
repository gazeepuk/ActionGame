// replcae later

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGPickUpBase.generated.h"

class USphereComponent;

UCLASS()
class ACTIONGAME_API AAGPickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAGPickUpBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pick Up|Interaction")
	TObjectPtr<USphereComponent> PickUpCollisionSphere;
	
	UFUNCTION()
	virtual void OnPickUpCollisionSphereBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnPickUpCollisionSphereEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
