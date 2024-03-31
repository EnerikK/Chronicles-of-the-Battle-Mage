// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BMAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void AbilityActorInfoSet();

protected:

	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle	ActiveEffectHandle);
	
};
