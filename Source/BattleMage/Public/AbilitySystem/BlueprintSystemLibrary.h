// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintSystemLibrary.generated.h"

class UAbilitySystemComponent;
class UOverlayWidgetController;
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

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass,float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
	
	
};
