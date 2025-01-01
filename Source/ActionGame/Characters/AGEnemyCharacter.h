// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGBaseCharacter.h"
#include "AGEnemyCharacter.generated.h"

class UEnemyCombatComponent;

UCLASS()
class ACTIONGAME_API AAGEnemyCharacter : public AAGBaseCharacter
{
	GENERATED_BODY()

public:
	AAGEnemyCharacter();

	//~ Begin IPawnCombatInterface Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface
protected:
	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End Begin APawn Interface
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UEnemyCombatComponent> EnemyCombatComponent;

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const {return EnemyCombatComponent; }

private:
	void InitEnemyStartUpData();
};