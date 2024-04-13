// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "BMInputConfig.generated.h"

struct FGameplayTag;

USTRUCT(BlueprintType)
struct FBattleMageInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag= FGameplayTag();

	
};
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UBMInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag,bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FBattleMageInputAction> AbilityInputAction;
	
};
