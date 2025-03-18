// replcae later


#include "AbilitySystem/AGAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Components/UI/HeroUIComponent.h"
#include "Components/UI/PawnUIComponent.h"
#include "CoreTypes/AGGameplayTags.h"
#include "FunctionLibraries/AGFunctionLibrary.h"
#include "Interfaces/PawnUIInterface.h"

UAGAttributeSet::UAGAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UAGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Get PawnUIInterface
	if(!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s doesn't implement IPawnInterface"), *GetNameSafe(GetOwningActor()));

	// Get PawnUIComponent
	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent, TEXT("PawnComponent of %s is invalid"), *GetNameSafe(GetOwningActor()));

	// CurrentHealth Attribute logic 
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		// Clamp Health value
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		// Broadcast CurrentHealth percent 
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	if(Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		// Clamp Rage value 
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

		SetCurrentRage(NewCurrentRage);

		// Add Rage State GameplayTag to owner
		if(GetCurrentRage() == GetMaxRage())
		{
			UAGFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), AGGameplayTags::Player_Status_Rage_Full);
		}
		else if(GetCurrentRage() == 0.f)
		{
			UAGFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), AGGameplayTags::Player_Status_Rage_None);
		}
		else
		{
			UAGFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), AGGameplayTags::Player_Status_Rage_Full);
			UAGFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), AGGameplayTags::Player_Status_Rage_None);
		}

		// Broadcast CurrentRage percent to Hero Character
		if(UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
		}
	}

	// DamageTaken Attribute logic 
	if(Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		// Calculate and clamp new CurrentHealth
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		// Broadcast CurrentHealth percent 
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		SetDamageTaken(0.f);

		// Add Death Status GameplayTag if CurrentHealth is zero
		if(GetCurrentHealth() == 0.f)
		{
			UAGFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), AGGameplayTags::Shared_Status_Death);
		}
	}
}
