// replcae later

#pragma once

#include "CoreMinimal.h"
#include "PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"


class AAGHeroWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	AAGHeroWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractingActor) override;
};
