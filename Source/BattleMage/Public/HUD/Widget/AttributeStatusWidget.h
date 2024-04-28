// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "HUD/BMWidgetController.h"
#include "AttributeStatusWidget.generated.h"

struct FGameplayAttribute;
struct FGameplayTag;
class UAttributeInfo;
struct FBattleMageAttributeInfo;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FBattleMageAttributeInfo& , Info);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class BATTLEMAGE_API UAttributeStatusWidget : public UBMWidgetController
{
	GENERATED_BODY()

public:
	
	virtual void BindCallBacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable,Category= "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable,Category= "GAS|Attributes")
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:

	void BroadCastAttributeInfo(const FGameplayTag& AttributeTag,const FGameplayAttribute& Attribute)const;
	
};
