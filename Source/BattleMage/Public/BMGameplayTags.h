// Hello

#pragma once


#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*BattleMage GameplayTags
Singleton containing native tags
*/

struct FBattleMageGameplayTags
{
public:
	static const FBattleMageGameplayTags& Get(){return GameplayTags;}
	static void InitializeNativeGameplayTags();

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;

	FGameplayTag Player_Block_CursorTrace;


private:

	static FBattleMageGameplayTags GameplayTags;
	
};
