// Hello :)


#include "AnimNotify/BM_AN_ResetState.h"

#include "Character/BMCharacter.h"

void UBM_AN_ResetState::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	
	if(MeshComp && MeshComp->GetOwner())
	{
		ControlledCharacter->ResetValues();
	}
	
}
