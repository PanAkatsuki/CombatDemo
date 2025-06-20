// Zhang All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace CombatGameplayTags
{
	/** Input Tags **/
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipAxe);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipAxe);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Axe);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Axe);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SpecialWeaponAbility_Light);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SpecialWeaponAbility_Heavy);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Roll);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SwitchTarget);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_PickUp_Stones);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld_Block);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld_Aim);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_TimeCounter);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CounterAttack);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable_TargetLock);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable_Rage);


	/** Player Tags **/
	// Weapon
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);

	// Ability
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Axe);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_Axe);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Axe);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Axe);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_SpecialWeaponAbility_Light);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_SpecialWeaponAbility_Heavy);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Roll);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Block);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_CounterAttack);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_TargetLock);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Rage);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_PickUp_Stones);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_HitPause);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Aim);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_TimeCounter);

	// Cooldown
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Cooldown_SpecialWeaponAbility_Light);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Cooldown_SpecialWeaponAbility_Heavy);

	// Event
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_HitPause);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SuccessfulBlock);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_CounterAttack);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SpecialAbilityAttack);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_AOE);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_ConsumeStones);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_ActivateRage);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SwithchTarget_Left);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SwithchTarget_Right);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_PerfectRoll);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_TimeCounter);

	// Status
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinisher);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_CanCounterAttack);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_CounterAttacking);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_SpecialAbilityAttacking);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rolling);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Blocking);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_TargetLock);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_Activating);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_Active);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_Full);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_None);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_TimeSlow);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_TimeCounter);

	// SetByCaller
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Light);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Heavy);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_CounterAttack);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_SpecialWeaponAbility);


	/** Enemy Tags **/
	// Weapon
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon);

	// Ability
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Melee);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Ranged);
	
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_SummonEnemies);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_SpawnStone);

	// Event
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Event_SummonEnemies);

	// Status
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Strafing);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_UnderAttack);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Unblockable);


	/** Shared Tags **/
	// Ability
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_HitReact);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Death);

	// Event
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_SpawnProjectile);

	// SetByCaller
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage);

	// Status
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Death);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Front);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Back);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Right);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Left);

	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Invincible);


	/** Game Data Tags **/
	// Level
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameData_Level_SurvivalGameModeMap);
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameData_Level_MainMenuMap);

	// SaveGame
	COMBAT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameData_SaveGame_Slot_1);

}
