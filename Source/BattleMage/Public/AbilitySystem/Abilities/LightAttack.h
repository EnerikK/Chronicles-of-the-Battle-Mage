// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BMDamageGameplayAbility.h"
#include "LightAttack.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ULightAttack : public UBMDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<ACharacter> OwnerPlayerCharacter;

	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<ABMCharacter> PlayerCharacter;
	
};
