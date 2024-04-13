// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BMGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,Category="Inputs")
	FGameplayTag StartupInputTag;
	
};
