// Hello :)

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BattleMageAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABattleMageAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABattleMageAIController();

protected:

	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
};
