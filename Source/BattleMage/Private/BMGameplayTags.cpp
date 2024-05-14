// Hello :)


#include "BMGameplayTags.h"
#include "GameplayTagsManager.h"

FBattleMageGameplayTags FBattleMageGameplayTags::GameplayTags;

void FBattleMageGameplayTags::InitializeNativeGameplayTags()
{
	/*Primary Attributes*/
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Strength"),
	FString("Increases physical damage"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Intelligence"),
	FString("Increases magical damage"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Resilience"),
	FString("Increases Armor and Armor Penetration"));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Vigor"),
	FString("Increases Health"));
	
	/*
	 * Secondary Attributes
	 */
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.Armor"),
	FString("Reduces damage taken, improves Block Chance"));
	
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.ArmorPenetration"),
	FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance"));
	
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.BlockChance"),
	FString("Chance to cut incoming damage in half"));
	
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.CriticalHitChance"),
	FString("Chance to double damage plus critical hit bonus"));
	
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.CriticalHitDamage"),
	FString("Bonus damage added when a critical hit is scored"));
	
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"),
	FString("Reduces Critical Hit Chance of attacking enemies")
	);
	
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.HealthRegeneration"),
	FString("Amount of Health regenerated every 1 second"));
	
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.ManaRegeneration"),
	FString("Amount of Mana regenerated every 1 second"));
	
	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.StaminaRegeneration"),
	FString("Amount of Stamina regenerated every 1 second"));
	
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.MaxHealth"),
	FString("Maximum amount of Health obtainable"));
	
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.MaxMana"),
	FString("Maximum amount of Mana obtainable"));
	
	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.MaxStamina"),
	FString("Maximum amount of Stamina obtainable"));

	/*Input tags*/
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.LMB"),
	FString("Input Tag for the Left mouse button"));
	
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.RMB"),
	FString("Input Tag for the Right mouse button"));
	
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.1"),
	FString("Input tag for the 1 Key"));
	
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.2"),
	FString("Input tag for the 2 Key"));
	
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.3"),
	FString("Input tag for the 3 Key"));
	
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.4"),
	FString("Input tag for the 4 Key"));

	/*
	* Combat sockets
	*/
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("CombatSocket.Weapon"),
	FString("WeaponAttacks"));
	
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("CombatSocket.RightHand"),
	FString("RightHands Attacks"));
	
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("CombatSocket.LeftHand"),
	FString("LeftHand Attacks"));
	
	/*
	* Player Tags
	*/
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Player.Block.CursorTrace"),
	FString("Block The trace under the cursor"));

	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Player.Block.InputHeld"),
	FString("Block The InputHeld buttons"));
	
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Player.Block.InputReleased"),
	FString("Block The InputReleased buttons"));
	
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Player.Block.InputPressed"),
	FString("Block The Input pressed buttons"));

	
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Damage"),
	FString("Damage"));

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Damage.Fire"),
	FString("Fire Damage"));

	GameplayTags.Damage_Earth = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Damage.Earth"),
	FString("Earth Damage"));
	
	GameplayTags.Damage_Water = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Damage.Water"),
	FString("Water Damage"));
	
	GameplayTags.Damage_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Damage.Wind"),
	FString("Wind Damage"));

	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Attributes.Resistance.Fire"),
	FString("Fire Damage Resistance"));

	GameplayTags.Attributes_Resistance_Water = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Attributes.Resistance.Water"),
	FString("Water Damage Resistance"));

	GameplayTags.Attributes_Resistance_Earth = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Attributes.Resistance.Earth"),
	FString("Earth Damage Resistance"));

	GameplayTags.Attributes_Resistance_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Attributes.Resistance.Wind"),
	FString("Wind Damage Resistance"));

	/*-_+ Mapping The type of damage to its resistance*/
	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Fire,GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Water,GameplayTags.Attributes_Resistance_Water);
	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Earth,GameplayTags.Attributes_Resistance_Earth);
	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Wind,GameplayTags.Attributes_Resistance_Wind);
	
	
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Effects.HitReact"),
	FString("HitReact"));

	
}