// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "BMAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
