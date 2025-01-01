// replcae later


#include "AGHeroGameplayAbility.h"

#include "Characters/AGHeroCharacter.h"
#include "Contrnollers/AGHeroPlayerController.h"

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
