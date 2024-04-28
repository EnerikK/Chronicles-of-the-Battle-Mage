// Hello :)


#include "HUD/Widget/AttributeStatusWidget.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/BMPlayerState.h"


void UAttributeStatusWidget::BindCallBacksToDependencies()
{
	/*check(AttributeInfo);
	for (auto& Pair : GetBattleMageAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadCastAttributeInfo(Pair.Key, Pair.Value());
		}
	);
	}
	
	GetBattleMagePS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);*/
	
}

void UAttributeStatusWidget::BroadcastInitialValues()
{
	UBMAttributeSet* AS = CastChecked<UBMAttributeSet>(AttributesSet);
	check(AttributeInfo);
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadCastAttributeInfo(Pair.Key, Pair.Value());
	}
	
}

void UAttributeStatusWidget::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UBMAbilitySystemComponent* BattleMageASC = CastChecked<UBMAbilitySystemComponent>(AbilitySystemComponent);
	BattleMageASC->UpgradeAttribute(AttributeTag);
}

void UAttributeStatusWidget::BroadCastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FBattleMageAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributesSet);
	AttributeInfoDelegate.Broadcast(Info);
}
