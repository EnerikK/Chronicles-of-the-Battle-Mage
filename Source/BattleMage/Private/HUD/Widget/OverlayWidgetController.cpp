// Hello :)


#include "HUD/Widget/OverlayWidgetController.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	
	/*OnHealthChanged.Broadcast(GetBattleMageAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetBattleMageAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetBattleMageAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetBattleMageAS()->GetMaxMana());
	OnStaminaChanged.Broadcast(GetBattleMageAS()->GetStamina());
	OnMaxStaminaChanged.Broadcast(GetBattleMageAS()->GetMaxStamina());*/
	
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
	
	Cast<UBMAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
	
}
