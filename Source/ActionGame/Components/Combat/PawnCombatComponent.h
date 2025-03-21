﻿// replcae later

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"


class AAGWeaponBase;

UENUM()
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AAGWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	AAGWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	AAGWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	virtual void OnHitTargetActor(AActor* HitActor);
	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractingActor);

	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	void ResetOverlappedActors(){ OverlappedActors.Empty(); }
protected:
	UPROPERTY()
	TArray<AActor*> OverlappedActors;
	
private:
	UPROPERTY()
	TMap<FGameplayTag, AAGWeaponBase*> CharacterCarriedWeaponMap;

	void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	virtual void ToggleHandsCollisions(bool bShouldEnable, EToggleDamageType ToggleDamageType);
};
