// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "AGHeroCharacter.generated.h"

class UHeroCombatComponent;
class UDataAsset_InputConfig;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONGAME_API AAGHeroCharacter : public AAGBaseCharacter
{
	GENERATED_BODY()

public:
	AAGHeroCharacter();
	
	//~ Begin IPawnCombatInterface Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

protected:
	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End Begin APawn Interface
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess))
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta=(AllowPrivateAccess))
	TObjectPtr<UHeroCombatComponent> HeroCombatComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta=(AllowPrivateAccess))
	TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);	
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
public:
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; }
};
