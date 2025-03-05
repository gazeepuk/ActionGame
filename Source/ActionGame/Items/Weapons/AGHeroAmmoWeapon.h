// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGHeroWeapon.h"
#include "AGHeroAmmoWeapon.generated.h"

UCLASS()
class ACTIONGAME_API AAGHeroAmmoWeapon : public AAGHeroWeapon
{
	GENERATED_BODY()

public:
	AAGHeroAmmoWeapon();
	
	UFUNCTION(BlueprintPure, Category = "Weapon|Ammo")
	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }

	UFUNCTION(BlueprintPure, Category = "Weapon|Ammo")
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure, Category = "Weapon|Ammo")
	bool HasEnoughAmmo(int32 InAmmoToSpend) const;

	UFUNCTION(BlueprintPure, Category = "Weapon|Ammo")
	bool ShouldReload() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	void ReloadAmmo();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData|Ammo", meta = (UIMin = "0"))
	int32 MaxAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|Ammo", meta = (UIMin = "0"))
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponData|Ammo")
	bool bInfiniteAmmo;
};
