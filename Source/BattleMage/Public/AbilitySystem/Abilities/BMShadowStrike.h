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

	UFUNCTION()
	void TimelineProgress(float value);

protected:

	UPROPERTY(EditAnywhere,Category="Timeline")
	UCurveFloat* CurveFloat;
	UPROPERTY()
	FVector StartLoc;
	UPROPERTY()
	FVector EndLoc;
	UPROPERTY(EditAnywhere,Category="Timeline")
	float ZOffset;
	
};
