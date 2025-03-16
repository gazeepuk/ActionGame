// replcae later

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"

class UAGWidgetBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UAGWidgetBase* InWidgetToRegister);
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgets();
	
private:
	TArray<UAGWidgetBase*> EnemyDrawnWidgets;
};
