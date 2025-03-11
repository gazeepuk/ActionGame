// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGHeroWeapon.h"
#include "AGHeroAmmoWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWeaponAmmoChanged, int32, CurrentAmmo, int32, MaxAmmo, bool, bInfinite);

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

	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	void SetCurrentAmmo(int32 InCurrentAmmo);
	
	UFUNCTION(BlueprintPure, Category = "Weapon|Ammo")
	bool HasEnoughAmmo(int32 InAmmoToSpend) const;

	UFUNCTION(BlueprintPure, Category = "Weapon|Ammo")
	bool ShouldReload() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	void ReloadAmmo();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	bool TryShoot(int32 InAmmoToSpend);

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Ammo")
	FOnWeaponAmmoChanged OnWeaponAmmoChanged;
	
private:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData|Ammo", meta = (UIMin = "0", AllowPrivateAccess = "true"))
	int32 MaxAmmo{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|Ammo", meta = (UIMin = "0", AllowPrivateAccess = "true"))
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponData|Ammo", meta = (AllowPrivateAccess = "true"))
	bool bInfiniteAmmo{false};
};
