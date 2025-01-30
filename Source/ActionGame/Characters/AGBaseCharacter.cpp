// replcae later


#include "AGBaseCharacter.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystem/AGAttributeSet.h"
#include "MotionWarpingComponent.h"

AAGBaseCharacter::AAGBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	AGAbilitySystemComponent = CreateDefaultSubobject<UAGAbilitySystemComponent>(TEXT("AGAbilitySystemComponent"));
	
	AGAttributeSet = CreateDefaultSubobject<UAGAttributeSet>(TEXT("AGAttributeSet"));

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("UMotionWarpingComponent"));
}

UAbilitySystemComponent* AAGBaseCharacter::GetAbilitySystemComponent() const
{
	return GetAGAbilitySystemComponent();
}

UPawnCombatComponent* AAGBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AAGBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

void AAGBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(AGAbilitySystemComponent)
	{
		AGAbilitySystemComponent->InitAbilityActorInfo(this, this);

		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Start up data is not assigned to %s"), *GetNameSafe(this));
	}
}
