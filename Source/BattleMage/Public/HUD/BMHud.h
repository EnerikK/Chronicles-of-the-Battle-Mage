// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BMHud.generated.h"

class UBMUserWidget;


/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMHud : public AHUD
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	TObjectPtr<UBMUserWidget> OverlayWidget;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	float MaxCrosshairSpread = 10.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
	
	
};
