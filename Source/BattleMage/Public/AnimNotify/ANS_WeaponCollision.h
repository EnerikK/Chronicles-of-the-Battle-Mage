// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_WeaponCollision.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UANS_WeaponCollision : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	
	
};
