// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*BattleMage GameplayTags
 *Singleton containing native tags
*/

struct FBattleMageGameplayTags
{
	
	static const FBattleMageGameplayTags& Get(){return GameplayTags;}
	static void InitializeNativeGameplayTags();

	/*AttribueTags*/
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_StaminaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_MaxStamina;
	
	/*Input Tags*/
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	
	FGameplayTag Player_Block_CursorTrace;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_InputPressed;

	/*_+DamageTags && Resistance Tags*/
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Water;
	FGameplayTag Damage_Earth;
	FGameplayTag Damage_Wind;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Water;
	FGameplayTag Attributes_Resistance_Earth;
	FGameplayTag Attributes_Resistance_Wind;


	TMap<FGameplayTag,FGameplayTag> DamageTypeToResistance;
	FGameplayTag Effects_HitReact;
	
	/*SocketTags*/
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;



private:

	static FBattleMageGameplayTags GameplayTags;
	
};
