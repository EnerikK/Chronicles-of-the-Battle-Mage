// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FBattleMageAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	FBattleMageAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag,bool bLogNotFound = false)const;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FBattleMageAttributeInfo> AttributeInformation;
	
};
