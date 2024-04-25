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

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
	
public:
	
	UTexture2D* CrosshairCenter;
	UTexture2D* CrosshairLeft;
	UTexture2D* CrosshairRight;
	UTexture2D* CrosshairTop;
	UTexture2D* CrosshairBottom;
	float CrosshairSpread;
	FLinearColor CrosshairColor;

};


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
	
	void InitOverlay(APlayerController* PC , APlayerState* PS , UAbilitySystemComponent* ASC , UAttributeSet* AS);
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeStatusWidget* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	
	virtual void DrawHUD() override;

	
	FORCEINLINE void SetHudPackage(const FHUDPackage& Package) {HudPackage = Package;}

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	APlayerController* OwningPlayer;
	
	FHUDPackage HudPackage;
	void DrawCrosshair(UTexture2D* Texture , FVector2d ViewportCenter,FVector2d Spread,FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float MaxCrosshairSpread = 10.f;

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
