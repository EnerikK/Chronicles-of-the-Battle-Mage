// Hello :)


#include "HUD/Widget/BMUserWidget.h"

void UBMUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
