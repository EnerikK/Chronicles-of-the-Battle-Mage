// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Types/CombatState.h"
#include "BMGameplayAbility.generated.h"

class UCombatComponent;
class ABMCharacter;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	ABMCharacter* ControlledPlayer;

	UPROPERTY()
	UCombatComponent* Combat;

	UFUNCTION(BlueprintPure)
	bool IsStateEqualToAny(const TArray<ECombatState>& StatesToCheck);
	UFUNCTION(BlueprintCallable)
	void SetState(ECombatState NewState);
	UFUNCTION(BlueprintPure)
	void GetState(ECombatState& ThisState);
	UPROPERTY(BlueprintReadOnly)
	ECombatState CurrentState;

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACharacter> OwnerCharacter;
	

	
	UPROPERTY(EditDefaultsOnly,Category="Inputs")
	FGameplayTag StartupInputTag;
	
};
