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

	if(!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s doesn't implement IPawnInterface"), *GetNameSafe(GetOwningActor()));

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent, TEXT("PawnComponent of %s is invalid"), *GetNameSafe(GetOwningActor()));
	
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	if(Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

		SetCurrentRage(NewCurrentRage);

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
		
		if(UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
		}
	}
	
	if(Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
		
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		SetDamageTaken(0.f);
		
		if(GetCurrentHealth() == 0.f)
		{
			UAGFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), AGGameplayTags::Shared_Status_Death);
		}
	}
}
