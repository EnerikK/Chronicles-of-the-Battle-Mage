// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "BMMotionWarping.generated.h"

class ABMCharacter;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMMotionWarping : public UMotionWarpingComponent
{
	GENERATED_BODY()

public:
	
	friend ABMCharacter;

	void AddOrUpdateWarpTargetFromLocationAndRotation(FName WarpTargetName, FVector TargetLocation, FRotator TargetRotation)
	{
		AddOrUpdateWarpTargetFromTransform(WarpTargetName, FTransform(TargetRotation, TargetLocation)); 
	}

private:

	UPROPERTY()
	ABMCharacter* Character;
	
};
