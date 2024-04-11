// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BattleMageCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABattleMageCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

		
public:

	ABattleMageCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	
	UPROPERTY()
	float CrouchBlendDuration = 0.5f;

	float CrouchBlendTime;
	
};
