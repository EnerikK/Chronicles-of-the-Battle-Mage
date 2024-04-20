// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BM_AN_SaveLightAttack.generated.h"

class ABMCharacter;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBM_AN_SaveLightAttack : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPROPERTY()
	ABMCharacter* ControlledCharacter;

	virtual void Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                             const FAnimNotifyEventReference& EventReference);
	
};
