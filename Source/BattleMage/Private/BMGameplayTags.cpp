// Hello

#include "BMGameplayTags.h"
#include "GameplayTagsManager.h"

FBattleMageGameplayTags FBattleMageGameplayTags::GameplayTags;

void FBattleMageGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.LMB"),
	FString("Input Tag for the Left mouse button"));
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.RMB"),
	FString("Input Tag for the Right mouse button"));
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.1"),
	FString("Input tag for the 1 Key"));
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.2"),
	FString("Input tag for the 2 Key"));
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("InputTag.3"),
	FString("Input tag for the 3 Key"));
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag
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

	
}
