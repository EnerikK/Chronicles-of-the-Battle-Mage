// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BMDamageGameplayAbility.h"
#include "BMChargeAttack.generated.h"

class AWeapon;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMChargeAttack : public UBMDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();
	
	ECombatState CombatState;

	UPROPERTY()
	UCombatComponent* Combat;

protected:
	
	UFUNCTION(BlueprintCallable,Category="MeleeAttack")
	virtual void GenerateAttack(const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget);
	
	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<ACharacter> OwnerPlayerCharacter;

	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<ABMCharacter> PlayerCharacter;
	
	
	
};
