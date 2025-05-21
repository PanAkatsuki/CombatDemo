// Zhang All Rights Reserved.


#include "Controllers/CombatHeroController.h"

ACombatHeroController::ACombatHeroController()
{
	HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId ACombatHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}
