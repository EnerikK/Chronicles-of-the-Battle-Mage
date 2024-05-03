// Hello :)


#include "AbilitySystem/Abilities/LightAttack.h"

#include "Character/BMCharacter.h"

void ULightAttack::StoreOwnerVariables()
{
	if(CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		PlayerCharacter = Cast<ABMCharacter>(CurrentActorInfo->AvatarActor);
	}
}
