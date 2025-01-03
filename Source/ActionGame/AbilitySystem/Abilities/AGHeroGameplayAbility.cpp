// replcae later


#include "AGHeroGameplayAbility.h"

#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "Characters/AGHeroCharacter.h"
#include "Contrnollers/AGHeroPlayerController.h"
#include "CoreTypes/AGGameplayTags.h"

AAGHeroCharacter* UAGHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if(!CachedHeroCharacter.IsValid())
	{
		CachedHeroCharacter = Cast<AAGHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedHeroCharacter.IsValid() ? CachedHeroCharacter.Get() : nullptr;
}

AAGHeroPlayerController* UAGHeroGameplayAbility::GetHeroPlayerControllerFromActorInfo()
{
	if(!CachedHeroPlayerController.IsValid())
	{
		CachedHeroPlayerController = Cast<AAGHeroPlayerController>(CurrentActorInfo->PlayerController);
	}
	return CachedHeroPlayerController.IsValid() ? CachedHeroPlayerController.Get() : nullptr;
}

UHeroCombatComponent* UAGHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return  GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

FGameplayEffectSpecHandle UAGHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag,
	int32 InUsedComboCount)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAGAbilitySystemComponentFromAvatarActor()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	FGameplayEffectSpecHandle EffectSpecHandle = GetAGAbilitySystemComponentFromAvatarActor()->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(AGGameplayTags::Shared_SetByCaller_BaseDamage, InWeaponBaseDamage);
	if(InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
	}
		
	return  EffectSpecHandle;
}
