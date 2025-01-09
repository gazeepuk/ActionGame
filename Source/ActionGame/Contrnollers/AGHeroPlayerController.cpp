// replcae later


#include "AGHeroPlayerController.h"

AAGHeroPlayerController::AAGHeroPlayerController()
{
	HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId AAGHeroPlayerController::GetGenericTeamId() const
{
	return HeroTeamID;
}
