// replcae later


#include "AGBaseCharacter.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystem/AGAttributeSet.h"
#include "MotionWarpingComponent.h"
#include "Components/BoxComponent.h"
#include "FunctionLibraries/AGFunctionLibrary.h"

AAGBaseCharacter::AAGBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	AGAbilitySystemComponent = CreateDefaultSubobject<UAGAbilitySystemComponent>(TEXT("AGAbilitySystemComponent"));
	
	AGAttributeSet = CreateDefaultSubobject<UAGAttributeSet>(TEXT("AGAttributeSet"));

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("UMotionWarpingComponent"));

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnHandCollisionBoxBeginOverlap);
	
	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnHandCollisionBoxBeginOverlap);
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

// Attach Hands' collisions in Editor
#if WITH_EDITOR
void AAGBaseCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandCollisionBoneName);
	}
	else if(PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandCollisionBoneName);
	}
}
#endif

void AAGBaseCharacter::OnHandCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if(UAGFunctionLibrary::IsTargetPawnHostile(this, HitPawn))
		{
			GetPawnCombatComponent()->OnHitTargetActor(HitPawn);
		}
	}
}
