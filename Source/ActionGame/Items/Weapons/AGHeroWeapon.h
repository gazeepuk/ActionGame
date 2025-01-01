// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGWeaponBase.h"
#include "GameplayAbilitySpecHandle.h"
#include "CoreTypes/AGStructTypes.h"
#include "AGHeroWeapon.generated.h"

struct FGameplayAbilitySpec;
class UBoxComponent;

UCLASS()
class ACTIONGAME_API AAGHeroWeapon : public AAGWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FAGHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitiesSpecHandles;
};
