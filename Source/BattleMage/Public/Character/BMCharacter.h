//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "Character/BMCharacterBase.h"
#include "BMCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMCharacter : public ABMCharacterBase
{
	GENERATED_BODY()

public:

	ABMCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
};
