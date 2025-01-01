// replcae later

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGWeaponBase.generated.h"

class UBoxComponent;

DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*)

UCLASS()
class ACTIONGAME_API AAGWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AAGWeaponBase();

	//Broadcast in OnCollisionBoxBeginOverlap
	FOnTargetInteractedDelegate OnWeaponHitTarget;
	//Broadcast in OnCollisionBoxEndOverlap
	FOnTargetInteractedDelegate OnWeaponPulledFromTarget;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	TObjectPtr<UBoxComponent> WeaponCollisionBox;

	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox; }
};
