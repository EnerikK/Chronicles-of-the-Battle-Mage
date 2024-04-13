// Hello :)


#include "Input/BMInputConfig.h"

const UInputAction* UBMInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FBattleMageInputAction& Action : AbilityInputAction)
	{
		if(Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp,Error,TEXT("Cant find abilityinput action for input tag [%s],on InputConfig[%s]"),*InputTag.ToString(),*GetNameSafe(this));
	}
	return nullptr;
}
