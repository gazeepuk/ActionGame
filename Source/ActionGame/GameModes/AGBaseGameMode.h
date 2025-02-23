// replcae later

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AGBaseGameMode.generated.h"

enum class EAGGameDifficulty : uint8;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API AAGBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAGBaseGameMode();

	UFUNCTION(BlueprintPure, Category = "Game Settings")
	FORCEINLINE EAGGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EAGGameDifficulty CurrentGameDifficulty;
};
