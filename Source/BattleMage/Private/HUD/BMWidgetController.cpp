// Hello :)


#include "HUD/BMWidgetController.h"

#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "Player/BMPlayerController.h"
#include "Player/BMPlayerState.h"

void UBMWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributesSet = WCParams.AttributeSet;
}

void UBMWidgetController::BroadcastInitialValues()
{
}

void UBMWidgetController::BindCallBacksToDependencies()
{
}

ABMPlayerController* UBMWidgetController::GetBattleMagePC()
{
	if(BMPlayerController == nullptr)
	{
		BMPlayerController = Cast<ABMPlayerController>(PlayerController);
	}
	return BMPlayerController;
}
ABMPlayerState* UBMWidgetController::GetBattleMagePS()
{
	if(BMPlayerState == nullptr)
	{
		BMPlayerState = Cast<ABMPlayerState>(PlayerState);
	}
	return BMPlayerState;
		
}

UBMAbilitySystemComponent* UBMWidgetController::GetBattleMageASC()
{
	if(BMAbilitySystemComponent == nullptr)
	{
		BMAbilitySystemComponent = Cast<UBMAbilitySystemComponent>(BMAbilitySystemComponent);
	}
	return BMAbilitySystemComponent;
}

UBMAttributeSet* UBMWidgetController::GetBattleMageAS()
{
	if(BMAttributeSet == nullptr)
	{
		BMAttributeSet = Cast<UBMAttributeSet>(BMAttributeSet);
	}
	return BMAttributeSet;
}
 