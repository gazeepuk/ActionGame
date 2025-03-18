// replcae later

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "AGGameInstance.generated.h"

USTRUCT(Blueprintable)
struct FAGGameLevelSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "GameData.Level"))
	FGameplayTag LevelTag;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const {return LevelTag.IsValid() && !Level.IsNull();}
};
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintPure, meta = (GameplayTagFilter = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InLevelTag);
	
protected:
	virtual void OnPreLoadMap(const FString& MapName);
	virtual void OnWorldLoaded(UWorld* LoadedWorld);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAGGameLevelSet> GameLevelSets;
};
