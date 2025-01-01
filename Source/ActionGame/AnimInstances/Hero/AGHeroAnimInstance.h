// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/AGCharacterAnimInstance.h"
#include "AGHeroAnimInstance.generated.h"

class AAGHeroCharacter;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGHeroAnimInstance : public UAGCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData")
	TObjectPtr<AAGHeroCharacter> OwningHeroCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;

	float IdleElapsedTime;
};
