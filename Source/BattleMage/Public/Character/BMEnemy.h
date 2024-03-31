// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Character/BMCharacterBase.h"
#include "BMEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMEnemy : public ABMCharacterBase
{
	GENERATED_BODY()

public:
	ABMEnemy();

protected:

	virtual void BeginPlay() override;
	
};
