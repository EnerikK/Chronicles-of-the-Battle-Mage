// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TextDamage.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UTextDamage : public UWidgetComponent
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetDamageText(float Damage);
	
};
