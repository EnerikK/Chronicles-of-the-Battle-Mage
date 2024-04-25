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


void ABMHud::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	Widget->AddToViewport();
}

void ABMHud::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));
	
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

void ABMHud::DrawHUD()
{
	Super::DrawHUD();

	FVector2d ViewportSize;
	if(GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		
		float SpreadScale = MaxCrosshairSpread* HudPackage.CrosshairSpread;
		
		if (HudPackage.CrosshairCenter)
		{
			FVector2d Spread(0.f,0.f);
			DrawCrosshair(HudPackage.CrosshairCenter, ViewportCenter,Spread,HudPackage.CrosshairColor);
		}
		if (HudPackage.CrosshairLeft)
		{
			FVector2d Spread(-SpreadScale,0.f);
			DrawCrosshair(HudPackage.CrosshairLeft, ViewportCenter,Spread,HudPackage.CrosshairColor);
		}
		if (HudPackage.CrosshairRight)
		{
			FVector2d Spread(SpreadScale,0.f);
			DrawCrosshair(HudPackage.CrosshairRight, ViewportCenter,Spread,HudPackage.CrosshairColor);
		}
		if (HudPackage.CrosshairTop)
		{
			FVector2d Spread(0.f,-SpreadScale);
			DrawCrosshair(HudPackage.CrosshairTop, ViewportCenter,Spread,HudPackage.CrosshairColor);
		}
		if (HudPackage.CrosshairBottom)
		{
			FVector2d Spread(0.f,SpreadScale);
			DrawCrosshair(HudPackage.CrosshairBottom, ViewportCenter,Spread,HudPackage.CrosshairColor);
		}
	}
}
void ABMHud::DrawCrosshair(UTexture2D* Texture, FVector2d ViewportCenter, FVector2d Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y);
	
	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor
	);
}

