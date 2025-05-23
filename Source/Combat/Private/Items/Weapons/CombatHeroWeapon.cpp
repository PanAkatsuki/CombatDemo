// Zhang All Rights Reserved.


#include "Items/Weapons/CombatHeroWeapon.h"

void ACombatHeroWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InAbilitySpecHandles)
{
	GrantedAbilitySpecHandles = InAbilitySpecHandles;
}

TArray<FGameplayAbilitySpecHandle>& ACombatHeroWeapon::GetGrantedAbilitySpecHandles()
{
	return GrantedAbilitySpecHandles;
}
