// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BMCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABmCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY()
	float CrouchBlendDuration = 0.5f;

	float CrouchBlendTime;
	
public:

	ABmCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	
};
