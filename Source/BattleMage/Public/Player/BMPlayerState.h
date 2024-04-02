// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "BMPlayerState.generated.h"

class ABMPlayerController;
class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMPlayerState : public APlayerState , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ABMPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	

protected:

	UPROPERTY()
	ABMPlayerController* Controller;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	
};
