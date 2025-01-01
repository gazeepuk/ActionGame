// replcae later


#include "AGCharacterAnimInstance.h"

#include "Characters/AGBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAGCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AAGBaseCharacter>(TryGetPawnOwner());

	if(OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UAGCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if(!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}
