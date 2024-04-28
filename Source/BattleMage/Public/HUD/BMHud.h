// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BMHud.generated.h"

class UAttributeStatusWidget;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
class UBMUserWidget;

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMHud : public AHUD
{
	GENERATED_BODY()

public:
	
	
	void InitOverlay(APlayerController* PC , APlayerState* PS , UAbilitySystemComponent* ASC , UAttributeSet* AS);
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeStatusWidget* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);


protected:


private:
	
	UPROPERTY()
	TObjectPtr<UBMUserWidget> OverlayWidget;

	UPROPERTY()
	APlayerController* OwningPlayer;
	

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeStatusWidget> AttributeStatusWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeStatusWidget> AttributeStatusWidgetClass;
	
	
};
