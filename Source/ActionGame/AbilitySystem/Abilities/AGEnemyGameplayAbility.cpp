// replcae later


#include "AGEnemyGameplayAbility.h"

#include "Characters/AGEnemyCharacter.h"

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
