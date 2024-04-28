// Hello :)


#include "HUD/BMHud.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Texture2D.h"
#include "HUD/BMWidgetController.h"
#include "HUD/Widget/AttributeStatusWidget.h"
#include "HUD/Widget/BMUserWidget.h"
#include "HUD/Widget/OverlayWidgetController.h"


void ABMHud::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_BMHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_BMHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UBMUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}

UOverlayWidgetController* ABMHud::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallBacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeStatusWidget* ABMHud::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeStatusWidget == nullptr)
	{
		AttributeStatusWidget = NewObject<UAttributeStatusWidget>(this, AttributeStatusWidgetClass);
		AttributeStatusWidget->SetWidgetControllerParams(WCParams);
		AttributeStatusWidget->BindCallBacksToDependencies();
	}
	return AttributeStatusWidget;
}
