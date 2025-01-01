// replcae later


#include "AGHeroAnimInstance.h"

#include "Characters/AGHeroCharacter.h"

void UAGHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if(OwningCharacter)
	{
		OwningHeroCharacter = Cast<AAGHeroCharacter>(OwningCharacter);
	}
}

void UAGHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if(bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElapsedTime >= EnterRelaxStateThreshold);
	}
}
