// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/AGBaseAnimInstance.h"
#include "AGHeroLinkedAnimLayer.generated.h"

class UAGHeroAnimInstance;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGHeroLinkedAnimLayer : public UAGBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta= (NotBlueprintThreadSafe))
	UAGHeroAnimInstance* GetHeroAnimInstance() const;
};
