// replcae later


#include "AGGameInstance.h"

#include "MoviePlayer.h"

void UAGGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnWorldLoaded);
}

void UAGGameInstance::OnPreLoadMap(const FString& MapName)
{
	// Set up Loading Screen
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
 
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void UAGGameInstance::OnWorldLoaded(UWorld* LoadedWorld)
{
	// Remove Loading Screen
	GetMoviePlayer()->StopMovie();
}

TSoftObjectPtr<UWorld> UAGGameInstance::GetGameLevelByTag(FGameplayTag InLevelTag)
{
	for (const FAGGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if(!GameLevelSet.IsValid())
		{
			continue;
		}

		if(GameLevelSet.LevelTag == InLevelTag)
		{
			return GameLevelSet.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}
