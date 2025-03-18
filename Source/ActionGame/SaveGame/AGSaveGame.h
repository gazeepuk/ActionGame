// replcae later

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/AGEnumTypes.h"
#include "GameFramework/SaveGame.h"
#include "AGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	EAGGameDifficulty SaveGameDifficulty;
};
