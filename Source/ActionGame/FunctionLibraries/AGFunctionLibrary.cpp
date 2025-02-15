// replcae later


#include "AGFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "CoreTypes/AGGameplayTags.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

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

