// Hello :)


#include "HUD/Widget/OverlayWidgetController.h"
#include "AbilitySystem/BMAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UBMAttributeSet* BattleMageAttributeSet = CastChecked<UBMAttributeSet>(AttributesSet);
	
	OnHealthChanged.Broadcast(BattleMageAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(BattleMageAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(BattleMageAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(BattleMageAttributeSet->GetMaxMana());
	OnStaminaChanged.Broadcast(BattleMageAttributeSet->GetStamina());
	OnMaxStaminaChanged.Broadcast(BattleMageAttributeSet->GetMaxStamina());
	
}

void UOverlayWidgetController::BindCallBacksToDependencies()
{
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(GetBattleMageAS()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);

		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(GetBattleMageAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);

		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(GetBattleMageAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);

		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(GetBattleMageAS()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);

		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(GetBattleMageAS()->GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStaminaChanged.Broadcast(Data.NewValue);

		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
	(GetBattleMageAS()->GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxStaminaChanged.Broadcast(Data.NewValue);

		});
}
