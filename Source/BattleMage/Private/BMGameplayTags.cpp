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
}
