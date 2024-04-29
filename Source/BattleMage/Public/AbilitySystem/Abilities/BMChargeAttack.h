// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BMDamageGameplayAbility.h"
#include "BMChargeAttack.generated.h"

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

protected:

	virtual void ActivateAbility
	(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void GenerateAttack(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget);
	
	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<ACharacter> OwnerPlayerCharacter;

	UPROPERTY(BlueprintReadWrite , Category = "Strike")
	TObjectPtr<ABMCharacter> PlayerCharacter;
	
	
	
};
