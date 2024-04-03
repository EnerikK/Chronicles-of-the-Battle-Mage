// Hello :)


#include "HUD/BMHud.h"
#include "Blueprint/UserWidget.h"



void ABMHud::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	Widget->AddToViewport();
}

