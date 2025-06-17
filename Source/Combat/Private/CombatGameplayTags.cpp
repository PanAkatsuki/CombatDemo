// Zhang All Rights Reserved.


#include "CombatGameplayTags.h"

namespace CombatGameplayTags
{
	/** Input Tags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, TEXT("InputTag.Move"));
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, TEXT("InputTag.Look"));

	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipAxe, TEXT("InputTag.EquipAxe"));
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipAxe, TEXT("InputTag.UnequipAxe"));

	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Axe, TEXT("InputTag.LightAttack.Axe"));
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_Axe, TEXT("InputTag.HeavyAttack.Axe"));

	UE_DEFINE_GAMEPLAY_TAG(InputTag_SpecialWeaponAbility_Light, TEXT("InputTag.SpecialWeaponAbility.Light"));
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SpecialWeaponAbility_Heavy, TEXT("InputTag.SpecialWeaponAbility.Heavy"));

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Roll, TEXT("InputTag.Roll"));
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchTarget, TEXT("InputTag.SwitchTarget"));

	UE_DEFINE_GAMEPLAY_TAG(InputTag_PickUp_Stones, TEXT("InputTag.PickUp.Stones"));

	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld, TEXT("InputTag.MustBeHeld"));
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld_Block, TEXT("InputTag.MustBeHeld.Block"));
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld_Aim, TEXT("InputTag.MustBeHeld.Aim"));

	UE_DEFINE_GAMEPLAY_TAG(InputTag_TimeCounter, TEXT("InputTag.TimeCounter"));


	UE_DEFINE_GAMEPLAY_TAG(InputTag_CounterAttack, TEXT("InputTag.CounterAttack"));

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable, TEXT("InputTag.Toggleable"));
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable_TargetLock, TEXT("InputTag.Toggleable.TargetLock"));
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable_Rage, TEXT("InputTag.Toggleable.Rage"));



	/** Player Tags **/
	// Weapon
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe, TEXT("Player.Weapon.Axe"));

	// Ability
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Axe, TEXT("Player.Ability.Equip.Axe"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Axe, TEXT("Player.Ability.Unequip.Axe"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Axe, TEXT("Player.Ability.Attack.Light.Axe"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Axe, TEXT("Player.Ability.Attack.Heavy.Axe"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_SpecialWeaponAbility_Light, TEXT("Player.Ability.SpecialWeaponAbility.Light"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_SpecialWeaponAbility_Heavy, TEXT("Player.Ability.SpecialWeaponAbility.Heavy"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Roll, TEXT("Player.Ability.Roll"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Block, TEXT("Player.Ability.Block"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_CounterAttack, TEXT("Player.Ability.CounterAttack"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_TargetLock, TEXT("Player.Ability.TargetLock"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Rage, TEXT("Player.Ability.Rage"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_PickUp_Stones, TEXT("Player.Ability.PickUp.Stones"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitPause, TEXT("Player.Ability.HitPause"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Aim, TEXT("Player.Ability.Aim"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_TimeCounter, TEXT("Player.Ability.TimeCounter"));


	// Cooldown
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_SpecialWeaponAbility_Light, TEXT("Player.Cooldown.SpecialWeaponAbility.Light"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_SpecialWeaponAbility_Heavy, TEXT("Player.Cooldown.SpecialWeaponAbility.Heavy"));

	// Event
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Axe, TEXT("Player.Event.Equip.Axe"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Axe, TEXT("Player.Event.Unequip.Axe"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitPause, TEXT("Player.Event.HitPause"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SuccessfulBlock, TEXT("Player.Event.SuccessfulBlock"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_CounterAttack, TEXT("Player.Event.CounterAttack"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SpecialAbilityAttack, TEXT("Player.Event.SpecialAbilityAttack"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_AOE, TEXT("Player.Event.AOE"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ConsumeStones, TEXT("Player.Event.ConsumeStones"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ActivateRage, TEXT("Player.Event.ActivateRage"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwithchTarget_Left, TEXT("Player.Event.SwithchTarget.Left"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwithchTarget_Right, TEXT("Player.Event.SwithchTarget.Right"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_PerfectRoll, TEXT("Player.Event.PerfectRoll"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_TimeCounter, TEXT("Player.Event.TimeCounter"));


	// Status
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_JumpToFinisher, TEXT("Player.Status.JumpToFinisher"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_CanCounterAttack, TEXT("Player.Status.CanCounterAttack"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_CounterAttacking, TEXT("Player.Status.CounterAttacking"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_SpecialAbilityAttacking, TEXT("Player.Status.SpecialAbilityAttacking"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rolling, TEXT("Player.Status.Rolling"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Blocking, TEXT("Player.Status.Blocking"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TargetLock, TEXT("Player.Status.TargetLock"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rage_Activating, TEXT("Player.Status.Rage.Activating"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rage_Active, TEXT("Player.Status.Rage.Active"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rage_Full, TEXT("Player.Status.Rage.Full"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rage_None, TEXT("Player.Status.Rage.None"));

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TimeSlow, TEXT("Player.Status.TimeSlow"));
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TimeCounter, TEXT("Player.Status.TimeCounter"));

	// SetByCaller
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Light, TEXT("Player.SetByCaller.AttackType.Light"));
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Heavy, TEXT("Player.SetByCaller.AttackType.Heavy"));
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_CounterAttack, TEXT("Player.SetByCaller.AttackType.CounterAttack"));
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_SpecialWeaponAbility, TEXT("Player.SetByCaller.AttackType.SpecialWeaponAbility"));


	/** Enemy Tags **/
	// Weapon
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, TEXT("Enemy.Weapon"));

	// Ability
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, TEXT("Enemy.Ability.Melee"));

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged, TEXT("Enemy.Ability.Ranged"));	

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SummonEnemies, TEXT("Enemy.Ability.SummonEnemies"));

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SpawnStone, TEXT("Enemy.Ability.SpawnStone"));

	// Event
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_SummonEnemies, TEXT("Enemy.Event.SummonEnemies"));

	// Status
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Strafing, TEXT("Enemy.Status.Strafing"));

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnderAttack, TEXT("Enemy.Status.UnderAttack"));

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Unblockable, TEXT("Enemy.Status.Unblockable"));


	/** Shared Tags **/
	// Ability
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, TEXT("Shared.Ability.HitReact"));
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, TEXT("Shared.Ability.Death"));

	// Event
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, TEXT("Shared.Event.MeleeHit"));
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, TEXT("Shared.Event.HitReact"));

	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SpawnProjectile, TEXT("Shared.Event.SpawnProjectile"));

	// SetByCaller
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, TEXT("Shared.SetByCaller.BaseDamage"));

	// Status
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Death, TEXT("Shared.Status.Death"));

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Front, TEXT("Shared.Status.HitReact.Front"));
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Back, TEXT("Shared.Status.HitReact.Back"));
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Right, TEXT("Shared.Status.HitReact.Right"));
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Left, TEXT("Shared.Status.HitReact.Left"));

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Invincible, TEXT("Shared.Status.Invincible"));

	/** Game Data Tags **/
	// Level
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_SurvivalGameModeMap, TEXT("GameData.Level.SurvivalGameModeMap"));
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_MainMenuMap, TEXT("GameData.Level.MainMenuMap"));

	// SaveGame
	UE_DEFINE_GAMEPLAY_TAG(GameData_SaveGame_Slot_1, TEXT("GameData.SaveGame.Slot.1"));

}
