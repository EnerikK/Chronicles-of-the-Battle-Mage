// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BMDamageGameplayAbility.h"
#include "BMShadowStrike.generated.h"


/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMShadowStrike : public UBMDamageGameplayAbility
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget();

	UPROPERTY(EditDefaultsOnly,Category="Inputs")
	FGameplayTag SecondaryInputTag;

	
protected:
	
	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<ABMCharacter> OwnerPlayerCharacter;

	/*UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<ACharacter> OwnerPlayerCharacter;*/

	

	
	
};
