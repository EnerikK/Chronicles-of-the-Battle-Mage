// Hello :)


#include "Types/CombatState.h"
#include "AbilitySystem/Abilities/BMGameplayAbility.h"


bool UBMGameplayAbility::IsStateEqualToAny(const TArray<ECombatState>& StatesToCheck)
{
	return StatesToCheck.Contains(CurrentState);
}

void UBMGameplayAbility::SetState(const ECombatState& NewState)
{
	if(NewState != CurrentState)
	{
		CurrentState = NewState;
	}
}

void UBMGameplayAbility::GetState(ECombatState& ThisState)
{
	CurrentState = ThisState;
}
