// replcae later


#include "HeroGameplayAbility_TargetLock.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/AGHeroCharacter.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widgets/AGWidgetBase.h"
#include "Controllers/AGHeroPlayerController.h"
#include "CoreTypes/AGGameplayTags.h"
#include "FunctionLibraries/AGFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	InitTargetLockMovement();
	InitTargetLockMappingContext();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
	CleanUp();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();

	AActor* NewTargetToLock = nullptr;
	
	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if(InSwitchDirectionTag.MatchesTagExact(AGGameplayTags::Player_Event_SwitchTarget_Left))
	{
		NewTargetToLock =  GetNearestActorFromAvailableActors(ActorsOnLeft);
	}
	else if(InSwitchDirectionTag.MatchesTagExact(AGGameplayTags::Player_Event_SwitchTarget_Right))
	{
		NewTargetToLock =  GetNearestActorFromAvailableActors(ActorsOnRight);
	}

	if(NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();
	if(AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestActorFromAvailableActors(AvailableActorsToLock);

	if(CurrentLockedActor)
	{
		DrawTargetLockWidget();
		SetTargetLockWidgetPosition();
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	AvailableActorsToLock.Empty();
	
	TArray<FHitResult> BoxTraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetAvatarActorFromActorInfo(),
		GetAvatarActorFromActorInfo()->GetActorLocation(), 
		GetAvatarActorFromActorInfo()->GetActorLocation() + GetAvatarActorFromActorInfo()->GetActorForwardVector() * TraceDistance,
		TraceBoxSize / 2.f,
		GetAvatarActorFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceObjectTypes,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true
		);

	for (const FHitResult& HitResult : BoxTraceHits)
	{
		if(AActor* HitActor = HitResult.GetActor(); HitActor && HitActor != GetAvatarActorFromActorInfo())
		{
			AvailableActorsToLock.AddUnique(HitActor);
		}
	} 
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestActorFromAvailableActors(const TArray<AActor*>& InAvailableActors) const
{
	if(InAvailableActors.Num() <= 0)
	{
		return nullptr;
	}
	float OutNearestDistance = -1.f;
	return UGameplayStatics::FindNearestActor(GetAvatarActorFromActorInfo()->GetActorLocation(), InAvailableActors, OutNearestDistance);
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft,
                                                                     TArray<AActor*>& OutActorsOnRight)
{
	if(!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	const FVector PlayerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		if(!AvailableActor || AvailableActor == CurrentLockedActor)
		{
			continue;
		}

		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		const FVector CrossProduct = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);
		if(CrossProduct.Z > 0.f)
		{
			OutActorsOnRight.AddUnique(AvailableActor);
		}
		else
		{
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
		
	}
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	// Set the TargetLock widget visible if exists
	if(TargetLockWidget.IsValid())
	{
		if(TargetLockWidget->GetVisibility() == ESlateVisibility::Hidden || TargetLockWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			TargetLockWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		return;
	}

	// Create a new TargetLock widget if does not exist
	checkf(TargetLockWidgetClass, TEXT("TargetLock widget class is not assigned"));
	TargetLockWidget = CreateWidget<UAGWidgetBase>(GetHeroPlayerControllerFromActorInfo(), TargetLockWidgetClass);

	check(TargetLockWidget.IsValid());
	TargetLockWidget->AddToViewport();
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	if(!TargetLockWidget.IsValid() || !CurrentLockedActor)
	{
		CancelTargetLockAbility();
		return;
	}

	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroPlayerControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true);

	if(TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		TargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* Widget)
			{
				if(USizeBox* FoundSizeBox = Cast<USizeBox>(Widget))
				{
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);
	}
	
	ScreenPosition -= (TargetLockWidgetSize / 2.f);
	
	TargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	if(!CurrentLockedActor ||
		!TargetLockWidget.IsValid() ||
		UAGFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor, AGGameplayTags::Shared_Status_Death) ||
		UAGFunctionLibrary::NativeDoesActorHaveTag(GetAvatarActorFromActorInfo(), AGGameplayTags::Shared_Status_Death))
	{
		CancelTargetLockAbility();
		return;
	}

	SetTargetLockWidgetPosition();

	const bool bShouldOverrideRotation = !UAGFunctionLibrary::NativeDoesActorHaveTag(GetAvatarActorFromActorInfo(), AGGameplayTags::Player_Event_Status_Rolling);

	if(bShouldOverrideRotation)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetAvatarActorFromActorInfo()->GetActorLocation(), CurrentLockedActor->GetActorLocation());

		LookAtRot -= FRotator(TargetLockCameraOffsetDistance, 0.f,0.f);
		
		const FRotator CurrentControlRot = GetHeroPlayerControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

		GetHeroPlayerControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		GetAvatarActorFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}

}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	check(TargetLockMappingContext);
	
	const ULocalPlayer* LocalPlayer = GetHeroPlayerControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(EnhancedInputSubsystem);

	EnhancedInputSubsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMappingContext()
{
	check(TargetLockMappingContext);

	if(!GetHeroPlayerControllerFromActorInfo())
	{
		return;
	}
	
	const ULocalPlayer* LocalPlayer = GetHeroPlayerControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(EnhancedInputSubsystem);

	EnhancedInputSubsystem->RemoveMappingContext(TargetLockMappingContext);
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();

	CurrentLockedActor = nullptr;

	if(TargetLockWidget.IsValid())
	{
		TargetLockWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	TargetLockWidgetSize = FVector2D::ZeroVector;

	CachedDefaultMaxWalkSpeed = 0.f;
}
