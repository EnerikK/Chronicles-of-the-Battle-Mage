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
	
	UPROPERTY(EditDefaultsOnly,Category="Inputs")
	FGameplayTag StartupInputTag;


protected:

	float GetManaCost(float InLevel = 1.f)const;
	float GetCooldown(float InLevel = 1.f)const;
	
	
};
