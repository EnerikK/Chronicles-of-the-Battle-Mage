// Hello :)


#include "AbilitySystem/Abilities/BMChargeAttack.h"
#include "Character/BMCharacter.h"

void UBMChargeAttack::StoreOwnerVariables()
{
	if(CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		PlayerCharacter = Cast<ABMCharacter>(CurrentActorInfo->AvatarActor);
	}
}

