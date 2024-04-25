// Hello :)


#include "AbilitySystem/BlueprintSystemLibrary.h"

#include "HUD/BMHud.h"
#include "HUD/BMWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BMPlayerState.h"

bool UBlueprintSystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
                                                         FWidgetControllerParams& OutWCParams, ABMHud*& OutBMHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutBMHUD = Cast<ABMHud>(PC->GetHUD());
		if (OutBMHUD)
		{
			ABMPlayerState* PS = PC->GetPlayerState<ABMPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UAttributeStatusWidget* UBlueprintSystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ABMHud* BMHud = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, BMHud))
	{
		return BMHud->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}
