// replcae later


#include "AbilitySystem/Abilities/AGGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "CoreTypes/AGGameplayTags.h"
#include "FunctionLibraries/AGFunctionLibrary.h"

void UAGGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	// Activates ability with OnGiven activation policy
	if (AbilityActivationPolicy == EAGAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UAGGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// Remove ability with OnGiven activation policy
	if (AbilityActivationPolicy == EAGAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UAGGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UAGAbilitySystemComponent* UAGGameplayAbility::GetAGAbilitySystemComponentFromAvatarActor() const
{
	return Cast<UAGAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UAGGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	check(TargetASC);
	check(InSpecHandle.IsValid());

	return GetAGAbilitySystemComponentFromAvatarActor()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UAGGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle, EAGSuccessType& OutSuccessType)
{
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EAGSuccessType::Successful : EAGSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}

bool UAGGameplayAbility::GetRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime,
	float& RemainingCooldownTime)
{
	check(InCooldownTag.IsValid());

	FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
	TArray<TPair<float,float>> TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if(!TimeRemainingAndDuration.IsEmpty())
	{
		RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
		TotalCooldownTime = TimeRemainingAndDuration[0].Value;
	}
	else
	{
		RemainingCooldownTime = 0.f;
		TotalCooldownTime = 0.f;
	}

	return  RemainingCooldownTime > 0.f;
}

void UAGGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle,
                                                                   const TArray<FHitResult>& HitResults, const bool bTargetHostile)
{
	if(HitResults.IsEmpty())
	{
		return;
	}

	APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());
	
	for (const FHitResult& HitResult : HitResults)
	{
		if(APawn* HitPawn = Cast<APawn>(HitResult.GetActor()))
		{
			// Apply EffectSpecHandle on Target HitActor if it's Hostile 
			if(UAGFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn) && bTargetHostile)
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = UAGFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(OwningPawn, HitPawn, InSpecHandle);

				// Send HitReact GameplayEvent if the Effect applied successfully
				if(ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{
					FGameplayEventData Data;
					Data.Target = HitPawn;
					Data.Instigator = OwningPawn;
					
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn, AGGameplayTags::Shared_Event_HitReact, Data);
				}
			}
		}
	}
}
