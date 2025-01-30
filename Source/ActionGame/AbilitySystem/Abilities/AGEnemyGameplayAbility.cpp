// replcae later


#include "AGEnemyGameplayAbility.h"

#include "AbilitySystem/AGAbilitySystemComponent.h"
#include "Characters/AGEnemyCharacter.h"
#include "CoreTypes/AGGameplayTags.h"

AAGEnemyCharacter* UAGEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if(!CachedEnemyCharacter.IsValid())
	{
		CachedEnemyCharacter = Cast<AAGEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UAGEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	AAGEnemyCharacter* EnemyCharacter = GetEnemyCharacterFromActorInfo();
	return EnemyCharacter ? EnemyCharacter->GetEnemyCombatComponent() : nullptr;
}

FGameplayEffectSpecHandle UAGEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat) const
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAGAbilitySystemComponentFromAvatarActor()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	FGameplayEffectSpecHandle EffectSpecHandle = GetAGAbilitySystemComponentFromAvatarActor()->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(AGGameplayTags::Shared_SetByCaller_BaseDamage, InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel()));
		
	return  EffectSpecHandle;
}
