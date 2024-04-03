// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BATTLEMAGE_API IEnemyInterface
{
	GENERATED_BODY()

public:
	
	virtual void Highlight() = 0;
	virtual void UnHighlight() = 0;

};
