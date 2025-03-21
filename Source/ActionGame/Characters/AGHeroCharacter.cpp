﻿// replcae later


#include "AGHeroCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/Input/AGEnhancedInputComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "CoreTypes/AGEnumTypes.h"
#include "CoreTypes/AGGameplayTags.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameModes/AGBaseGameMode.h"

AAGHeroCharacter::AAGHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));

	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}

UPawnCombatComponent* AAGHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AAGHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

UHeroUIComponent* AAGHeroCharacter::GetHeroUIComponent() const
{
	return HeroUIComponent;
}

void AAGHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!CharacterStartUpData.IsNull())
	{
		if(UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			int32 AbilityApplyLevel = 1;
			
			if(AAGBaseGameMode* BaseGameMode = GetWorld()->GetAuthGameMode<AAGBaseGameMode>())
			{
				switch (BaseGameMode->GetCurrentGameDifficulty())
				{
				case EAGGameDifficulty::Easy:
					AbilityApplyLevel = 1;
					break;
				case EAGGameDifficulty::Normal:
					AbilityApplyLevel = 2;
					break;
				case EAGGameDifficulty::Hard:
					AbilityApplyLevel = 3;
					break;
				case EAGGameDifficulty::VeryHard:
					AbilityApplyLevel = 4;
					break;
				default:
					break;
				}
			}
			
			LoadedData->GiveToAbilitySystemComponent(AGAbilitySystemComponent, AbilityApplyLevel);
		}
	}
}

void AAGHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	checkf(InputConfigDataAsset, TEXT("Input config DataAsset is nullptr"));

	const ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(EnhancedInputSubsystem);

	EnhancedInputSubsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UAGEnhancedInputComponent* AGInputComponent = CastChecked<UAGEnhancedInputComponent>(InputComponent);

	AGInputComponent->BindNativeInputAction(InputConfigDataAsset, AGGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	AGInputComponent->BindNativeInputAction(InputConfigDataAsset, AGGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	AGInputComponent->BindNativeInputAction(InputConfigDataAsset, AGGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	AGInputComponent->BindNativeInputAction(InputConfigDataAsset, AGGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	AGInputComponent->BindNativeInputAction(InputConfigDataAsset, AGGameplayTags::InputTag_PickUp_Stones, ETriggerEvent::Started, this, &ThisClass::Input_PickUpStoneStarted);

	AGInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}


void AAGHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if(MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if(MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAGHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if(LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if(LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAGHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AAGHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X > 0.f ? AGGameplayTags::Player_Event_SwitchTarget_Right : AGGameplayTags::Player_Event_SwitchTarget_Left,
		FGameplayEventData());
}

void AAGHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	AGAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AAGHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	AGAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void AAGHeroCharacter::Input_PickUpStoneStarted(const FInputActionValue& InputActionValue)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		AGGameplayTags::Player_Event_ConsumeStone,
		FGameplayEventData());
}
