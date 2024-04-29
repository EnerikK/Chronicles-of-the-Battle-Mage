//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BMGameModeBase.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,Category="Character Class Info")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
};
