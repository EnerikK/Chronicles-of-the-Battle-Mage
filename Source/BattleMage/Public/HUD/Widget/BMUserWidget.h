// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BMUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};
