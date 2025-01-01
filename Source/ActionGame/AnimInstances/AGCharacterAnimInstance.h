// replcae later

#pragma once

#include "CoreMinimal.h"
#include "AGBaseAnimInstance.h"
#include "AGCharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class AAGBaseCharacter;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGCharacterAnimInstance : public UAGBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	TObjectPtr<AAGBaseCharacter> OwningCharacter;
	TObjectPtr<UCharacterMovementComponent> OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;
};
