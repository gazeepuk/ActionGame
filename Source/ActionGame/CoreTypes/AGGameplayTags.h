// replcae later

#pragma once

#include "NativeGameplayTags.h"

namespace AGGameplayTags
{
	/** Input Tags **/
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);

	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Roll);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SwitchTarget);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SpecialWeaponAbility_Light);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SpecialWeaponAbility_Heavy);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipWeapon);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipWeapon);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Axe);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Pistol);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Axe);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Pistol);

	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MustBeHeld_Block);

	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable_TargetLock);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Toggleable_Rage);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_PickUp_Stones);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Reload);

	/** Player Tags **/
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Axe);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_Axe);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Axe);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Axe);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_HitPause);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Roll);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Block);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_TargetLock);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_SpecialWeaponAbility_Light);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_SpecialWeaponAbility_Heavy);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Rage);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_PickUp_Stone);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Cooldown_SpecialWeaponAbility_Light);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Cooldown_SpecialWeaponAbility_Heavy);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Pistol);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_HitPause);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SuccessfulBlock);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SwitchTarget_Left);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_SwitchTarget_Right);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_ActivateRage);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_AOE);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_ConsumeStone);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Ammo_Reload);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Disappear);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Appear);

	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Status_JumpToFinisher);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Status_Rolling);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Blocking);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_TargetLock);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_Activating);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_Active);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_Full);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Rage_None);

	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Ammo_Empty);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_Ammo_Reloading);

	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Light);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Heavy);
	
		
	
	/** Enemy Tags **/
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon);

	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Melee);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Ranged);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_SummonEnemies);

	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Strafing);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_UnderAttack);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Unblockable);

	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Event_SummonEnemies);
	
	/** Shared Tags **/
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_HitReact);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Death);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_SpawnStone);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_SpawnProjectile);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage);

	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Death);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Invincible);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Front);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Back);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Left);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Right);

	/** Game Data Tags**/
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameData_Level_SurvivalGameModeMap);
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameData_Level_MainMenuMap);
	
	ACTIONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameData_Save_Slot_1);
	
}