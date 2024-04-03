// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Character/BMCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "BMEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMEnemy : public ABMCharacterBase , public IEnemyInterface
{
	GENERATED_BODY()

public:
	ABMEnemy();

	/*EnemyInterface*/
	virtual void Highlight() override;
	virtual void UnHighlight() override;
	/*End EnemyInterface*/

protected:

	virtual void BeginPlay() override;
	
	
};
