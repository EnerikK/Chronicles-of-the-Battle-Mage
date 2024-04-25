// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BMWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);

class UBMAttributeSet;
class UBMAbilitySystemComponent;
class ABMPlayerState;
class ABMPlayerController;
class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC,APlayerState*PS,UAbilitySystemComponent* ASC,UAttributeSet* AS):
	PlayerController(PC) ,PlayerState(PS) , AbilitySystemComponent(ASC) , AttributeSet(AS) {}

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent>  AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	
};
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMWidgetController : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BindCallBacksToDependencies();

protected:

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributesSet;

		
	UPROPERTY(BlueprintReadOnly , Category= "WidgetController")
	TObjectPtr<ABMPlayerController> BMPlayerController;

	UPROPERTY(BlueprintReadOnly , Category= "WidgetController")
	TObjectPtr<ABMPlayerState> BMPlayerState;
	
	UPROPERTY(BlueprintReadOnly , Category= "WidgetController")
	TObjectPtr<UBMAbilitySystemComponent>	BMAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly , Category= "WidgetController")
	TObjectPtr<UBMAttributeSet>	BMAttributeSet;

	ABMPlayerController* GetBattleMagePC();
	ABMPlayerState* GetBattleMagePS();
	UBMAbilitySystemComponent* GetBattleMageASC();
	UBMAttributeSet* GetBattleMageAS();
	
	
};
