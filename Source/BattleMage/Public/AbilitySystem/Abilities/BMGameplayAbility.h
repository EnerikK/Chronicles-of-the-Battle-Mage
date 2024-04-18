// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Types/CombatState.h"
#include "BMGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	bool IsStateEqualToAny(const TArray<ECombatState>& StatesToCheck);
	UFUNCTION(BlueprintCallable)
	void SetState(const ECombatState& NewState);
	UFUNCTION(BlueprintPure)
	void GetState(ECombatState& ThisState);
	UPROPERTY(BlueprintReadOnly)
	ECombatState CurrentState;

	void PerformAttack();
	
	UPROPERTY(EditDefaultsOnly,Category="Inputs")
	FGameplayTag StartupInputTag;
	
};
