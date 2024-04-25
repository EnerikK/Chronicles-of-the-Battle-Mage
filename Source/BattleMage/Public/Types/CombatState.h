// Hello :) 

#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECState_Unoccupied		 UMETA(DisplayName = "Unoccupied"),
	ECState_ThrowGrenade	 UMETA(DisplayName = "ThrowGrenade"),
	ECState_Reloading		 UMETA(DisplayName = "ThrowGrenade"),
	ECState_Slide			 UMETA(DisplayName = "Slide"),
	ECState_Crouch 			 UMETA(DisplayName = "Crouch"),
	ECState_Attack			 UMETA(DisplayName = "Attack"),
	ECState_HeavyAttack		 UMETA(DisplayName = "HeavyAttack"),
	ECState_ChargeAttack	 UMETA(DisplayName = "ChargeAttack"),
	ECState_SwapWeapons		 UMETA(DisplayName = "SwapWeapons"),
	
	ECState_MAX				 UMETA(DisplayName = "DefaultMax"),

};