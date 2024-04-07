//Hello :)

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "BMPlayerController.generated.h"

class UBMAbilitySystemComponent;
class IEnemyInterface;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class BATTLEMAGE_API ABMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ABMPlayerController();
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:


	UBMAbilitySystemComponent* GetASC();

	UPROPERTY()
	TObjectPtr<UBMAbilitySystemComponent> AuraAbilitySystemComponent;

	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> BMContest;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> EquipAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);



	
};
