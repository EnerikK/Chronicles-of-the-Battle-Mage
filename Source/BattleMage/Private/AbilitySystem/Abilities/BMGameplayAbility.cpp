// Hello :)


#include "AbilitySystem/Abilities/BMGameplayAbility.h"

#include "Character/BMCharacter.h"
#include "Interaction/CombatComponent.h"
#include "Weapon/Weapon.h"


bool UBMGameplayAbility::IsStateEqualToAny(const TArray<ECombatState>& StatesToCheck)
{
	return StatesToCheck.Contains(CurrentState);
}

void UBMGameplayAbility::SetState(ECombatState NewState)
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

void UBMGameplayAbility::StoreOwnerVariables()
{
	if(CurrentActorInfo)
	{
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}
