// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintSystemLibrary.generated.h"

struct FWidgetControllerParams;
class ABMHud;
class UAttributeStatusWidget;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBlueprintSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*WidgetController*/
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ABMHud*& OutBMHUD);

	UFUNCTION(BlueprintPure,Category = "AuraAbilitySystemLibrary | WidgeController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeStatusWidget* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	
};
