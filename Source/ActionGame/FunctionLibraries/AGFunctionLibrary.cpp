// replcae later


#include "AGFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "CoreTypes/AGCountDownAction.h"
#include "CoreTypes/AGGameplayTags.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "GameFramework/SaveGame.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameInstances/AGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/AGSaveGame.h"

UAGAbilitySystemComponent* UAGFunctionLibrary::NativeGetAGAbilitySystemComponentFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UAGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UAGFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UAGAbilitySystemComponent* ASC = NativeGetAGAbilitySystemComponentFromActor(InActor);
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UAGFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UAGAbilitySystemComponent* ASC = NativeGetAGAbilitySystemComponentFromActor(InActor);
	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UAGFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UAGAbilitySystemComponent* ASC = NativeGetAGAbilitySystemComponentFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UAGFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EAGConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EAGConfirmType::Yes : EAGConfirmType::No;
}

UPawnCombatComponent* UAGFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	if (!InActor)
	{
		return nullptr;
	}
	if (const IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}

UPawnCombatComponent* UAGFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EAGValidType& OutValidType)
{
	UPawnCombatComponent* PawnCombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = PawnCombatComponent ? EAGValidType::Valid : EAGValidType::InValid;

	return PawnCombatComponent;
}

bool UAGFunctionLibrary::IsTargetPawnHostile(const APawn* QueryPawn, const APawn* TargetPawn)
{
	// Check Team ID to calculate if Target is Hostile
	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if(QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

float UAGFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UAGFunctionLibrary::ComputeHitReactDirectionTag(const AActor* InAttacker, const AActor* InVictim,
                                                             float& OutAngleDifference)
{
	check(InAttacker && InVictim);

	const FVector VictimForward	= InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	// Compute Dot and Cross products to find angle between Attacker and Victim in degrees
	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(VictimForward,VictimToAttackerNormalized);
	if(CrossResult.Z < 0.f)
	{
		OutAngleDifference *= -1.f;
	}

	if(OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
	{
		return AGGameplayTags::Shared_Status_HitReact_Front;
	}
	else if(OutAngleDifference < -45.f && OutAngleDifference >= -135.f)
	{
		return AGGameplayTags::Shared_Status_HitReact_Left;
	}
	else if(OutAngleDifference > -45.f && OutAngleDifference <= 135.f)
	{
		return AGGameplayTags::Shared_Status_HitReact_Right;
	}
	else if(OutAngleDifference < -135.f || OutAngleDifference > 135.f)
	{
		return AGGameplayTags::Shared_Status_HitReact_Back;
	}
	
	return AGGameplayTags::Shared_Status_HitReact_Front;
}

bool UAGFunctionLibrary::IsValidBlock(const AActor* InAttacker, const AActor* InDefender)
{
	check(InAttacker && InDefender);

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

	return DotResult < -0.25f;
}

bool UAGFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InInstigator);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTargetActor);

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UAGFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval,
                                   float& OutRemainingTime, EAGCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EAGCountDownActionOutput& CountDownOutput,
                                   FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;

	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
	}

	if (!World)
	{
		return;
	}

	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

	FAGCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FAGCountDownAction>(LatentInfo.CallbackTarget,LatentInfo.UUID);

	if (CountDownInput == EAGCountDownActionInput::Start)
	{
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget,
				LatentInfo.UUID,
				new FAGCountDownAction(TotalTime,UpdateInterval,OutRemainingTime,CountDownOutput,LatentInfo)
			);
		}
	}

	if (CountDownInput == EAGCountDownActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}

UInputAction* UAGFunctionLibrary::GetAbilityInputActionByTagFromInputConfigDataAsset(
	const UDataAsset_InputConfig* InInputConfigDataAsset, const FGameplayTag& InGameplayTag)
{
	return InInputConfigDataAsset->FindAbilityInputActionByTag(InGameplayTag);
}

UAGGameInstance* UAGFunctionLibrary::GetAGGameInstance(const UObject* WorldContextObject)
{
	if(GEngine)
	{
		if(const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			return World->GetGameInstance<UAGGameInstance>();
		}
	}

	return nullptr;
}

void UAGFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EAGInputMode InInputMode)
{
	APlayerController* PlayerController = nullptr;
	if(GEngine)
	{
		if(const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}

	if(!PlayerController)
	{
		return;
	}

	FInputModeGameOnly InputModeGameOnly;
	FInputModeUIOnly InputModeUIOnly;
	
	switch (InInputMode)
	{
	case EAGInputMode::GameOnly:
		PlayerController->SetInputMode(InputModeGameOnly);
		PlayerController->bShowMouseCursor = false;
		break;
		
	case EAGInputMode::UIOnly:
		PlayerController->SetInputMode(InputModeUIOnly);
		PlayerController->bShowMouseCursor = true;
		break;
		
	default:
		break;
	}
}

void UAGFunctionLibrary::SaveCurrentGameDifficulty(EAGGameDifficulty InGameDifficulty)
{
	USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UAGSaveGame::StaticClass());

	if(UAGSaveGame* AGSaveGame = Cast<UAGSaveGame>(SaveGame))
	{
		AGSaveGame->SaveGameDifficulty = InGameDifficulty;

		UGameplayStatics::SaveGameToSlot(AGSaveGame, AGGameplayTags::GameData_Save_Slot_1.GetTag().ToString(), 0);
	}
}

bool UAGFunctionLibrary::TryLoadCurrentGameDifficulty(EAGGameDifficulty& OutGameDifficulty)
{
	if(UGameplayStatics::DoesSaveGameExist(AGGameplayTags::GameData_Save_Slot_1.GetTag().ToString(), 0))
	{
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(AGGameplayTags::GameData_Save_Slot_1.GetTag().ToString(), 0);
		if(UAGSaveGame* AGSaveGame = Cast<UAGSaveGame>(SaveGame))
		{
			OutGameDifficulty = AGSaveGame->SaveGameDifficulty;
			return true;
		}
	}

	OutGameDifficulty = EAGGameDifficulty::Easy;
	return false;
}

